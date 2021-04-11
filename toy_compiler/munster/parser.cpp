/**
 * @file parser.cpp
 * @brief Implement the functions defined at parser.hpp
 * @copyright Copyright (C) 2021 wmbat.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <toy_compiler/munster/parser.hpp>

#include <toy_compiler/munster/grammar/production.hpp>
#include <toy_compiler/munster/grammar/production_table.hpp>
#include <toy_compiler/munster/grammar/symbol.hpp>

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/functional/not_fn.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/take_while.hpp>

#include <iostream>

namespace munster
{
   namespace vi = ranges::views;

   auto construct_symbol_table() -> const grammar::production_table;
   auto is_comment(const lex_item& item) -> bool;
   auto is_epsilon(const grammar::symbol& s) -> bool { return s == grammar::token_type::e_epsilon; }
   auto pop(std::vector<grammar::symbol>& stack) -> grammar::symbol
   {
      auto temp = *(std::end(stack) - 1);
      stack.pop_back();
      return temp;
   }

   auto check_follow_sets(grammar::token_type type, const grammar::symbol& top) -> bool
   {
      const auto tail =
         ranges::find(grammar::sets::follow, top, &grammar::production::start)->tail();

      return ranges::find(tail, type) != std::end(tail);
   }

   auto contains(const grammar::symbol_array& tail, grammar::token_type type) -> bool
   {
      return ranges::find(tail, type) != std::end(tail);
   }

   auto parse_impl(std::span<const lex_item> items, util::logger_wrapper log) -> parse_result
   {
      static const auto table = grammar::construct_production_table();

      std::vector<parse_error> errors;
      std::vector<munster::ast::node_ptr> nodes;
      std::vector<grammar::symbol> stack;
      stack.push_back(grammar::symbol::stop());
      stack.push_back(grammar::symbol::start());

      std::string derivation;
      std::string parsed_tokens;
      std::string final_derivations;

      const auto action_filter = [](grammar::symbol& s) {
         return !grammar::is_action(s);
      };

      auto item_it = std::begin(items);
      while (stack.back() != grammar::symbol::stop())
      {
         derivation =
            fmt::format("{}", stack | vi::reverse | vi::filter(action_filter) | ranges::to_vector);

         const auto& top_symbol = stack.back();

         if (grammar::is_terminal(top_symbol))
         {
            if (!grammar::is_eof(item_it->type) && top_symbol == item_it->type)
            {
               log.info("Parsed token: {}", *item_it);

               parsed_tokens += fmt::format("{} ", item_it->lexeme);

               stack.pop_back();
               ++item_it;
            }
            else
            {
               if (grammar::is_eof(item_it->type))
               {
                  log.info("symbol {} popped from stack", top_symbol);

                  stack.pop_back();
               }
               else
               {
                  log.warning("SCANNING...");

                  const auto type = get<grammar::symbol_type::terminal>(top_symbol);
                  errors.push_back(parse_error{.type = parse_error_type::e_syntax_error,
                                               .pos = item_it->pos,
                                               .lexeme = fmt::format("{}", type),
                                               .line = {}});

                  while (!grammar::is_eof(item_it->type) && item_it->type != top_symbol)
                  {
                     log.warning("\t{} : {}", top_symbol, *item_it);

                     ++item_it;
                  }
               }
            }
         }
         else if (grammar::is_non_terminal(top_symbol))
         {
            const auto head = get<grammar::symbol_type::non_terminal>(top_symbol);
            const auto& production = table.lookup({head, item_it->type});
            if (production != grammar::production{})
            {
               const auto symbols = production.tail() | ranges::views::reverse | ranges::to_vector;

               log.info("Applying derivation: {}", grammar::production{head, symbols});

               stack.pop_back();

               for (auto& symbol : symbols | vi::filter(ranges::not_fn(is_epsilon)))
               {
                  stack.push_back(symbol);
               }
            }
            else
            {
               if (grammar::is_eof(item_it->type) || check_follow_sets(item_it->type, top_symbol))
               {
                  log.info("symbol {} popped from stack", top_symbol);
                  stack.pop_back();
               }
               else
               {
                  const auto& first_it =
                     ranges::find(grammar::sets::first, top_symbol, &grammar::production::start);
                  const auto& follow_it =
                     ranges::find(grammar::sets::follow, top_symbol, &grammar::production::start);

                  log.warning("SCANNING OF {}...", top_symbol);

                  const auto type = get<grammar::symbol_type::non_terminal>(top_symbol);
                  errors.push_back(parse_error{.type = parse_error_type::e_syntax_error,
                                               .pos = item_it->pos,
                                               .lexeme = fmt::format("{}", type),
                                               .line = {}});

                  if (first_it->nullable())
                  {
                     while (!grammar::is_eof(item_it->type) &&
                            !contains(follow_it->tail(), item_it->type))
                     {
                        log.warning("\t{} : {}", top_symbol, *item_it);

                        ++item_it;
                     }
                  }
                  else
                  {
                     while (!grammar::is_eof(item_it->type) &&
                            !contains(first_it->tail(), item_it->type))
                     {
                        log.warning("\t{} : {}", top_symbol, *item_it);

                        ++item_it;
                     }
                  }
               }
            }
         }
         else
         {
            const auto type = grammar::get_action_type(top_symbol);

            stack.pop_back();
            if (item_it != std::begin(items))
            {
               nodes.push_back(munster::ast::node_factory(type, *(item_it - 1), nodes));
            }
            else
            {
               nodes.push_back(munster::ast::node_factory(type, {}, nodes));
            }
         }

         final_derivations += fmt::format("{}{}\n", parsed_tokens, derivation);
      }

      if (!grammar::is_eof(item_it->type) || !std::empty(errors))
      {
         return {.value = parse_status::error,
                 .ast = std::move(nodes.back()),
                 .derivation = final_derivations,
                 .errors = errors};
      }

      return {.value = parse_status::success,
              .ast = std::move(nodes.back()),
              .derivation = final_derivations,
              .errors = std::nullopt};
   }

   auto parse_items(std::span<const lex_item> items, util::logger_wrapper log) -> parse_result
   {
      const auto cleaned = items | vi::filter(ranges::not_fn(is_comment)) | ranges::to_vector;

      return parse_impl(cleaned, log);
   }

   auto is_comment(const lex_item& item) -> bool
   {
      using namespace grammar;
      return (item.type == grammar::token_type::e_block_cmt) ||
         (item.type == grammar::token_type::e_line_cmt);
   }
} // namespace munster
