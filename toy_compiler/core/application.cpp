/**
 * @file application.cpp
 * @brief Implement the functions defined at application.hpp
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

#include "toy_compiler/munster/visitor/symbol_table/memory_size_visitor.hpp"
#include <toy_compiler/core/application.hpp>

#include <toy_compiler/munster/visitor/semantic_checking/type_checking_visitor.hpp>
#include <toy_compiler/munster/visitor/symbol_table/symbol_table_visitor.hpp>
#include <toy_compiler/munster/visitor/visitor.hpp>

#include <fmt/color.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/view/map.hpp>

#include <mpark/patterns.hpp>

#include <cassert>
#include <fstream>

namespace fs = std::filesystem;

auto pop(std::vector<munster::ast::node*>& stack) -> munster::ast::node*
{
   auto temp = *(std::end(stack) - 1);
   stack.pop_back();
   return temp;
}

void print_errors(std::span<const munster::parse_error> errors,
                  const std::filesystem::path& filepath)
{
   for (const auto& err : errors)
   {
      fmt::print(fmt::emphasis::bold, "{}:{}.{} - ", filepath.c_str(), err.pos.line,
                 err.pos.column);
      if (err.type == munster::parse_error_type::e_semantic_error)
      {
         fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "[{}] ", err.type);
      }
      else
      {
         fmt::print(fmt::fg(fmt::color::yellow) | fmt::emphasis::bold, "[{}] ", err.type);
      }

      fmt::print("{}\n", err.lexeme);

      fmt::print("{}\n", err.line);
   }
}

application::application(std::span<const std::string_view> args, util::logger_wrapper log) :
   m_logger{log}
{
   for (const auto filename : args)
   {
      const auto filepath = fs::path{filename};
      if (filepath.extension() == ".src")
      {
         if (auto maybe = munster::lex_file(filepath, m_logger))
         {
            const auto result = munster::parse_items(maybe.value(), m_logger);
            if (result.value == munster::parse_status::error)
            {
               print_errors(result.errors.value(), filepath);
            }

            write_lexing_to_file(filepath, maybe.value());
            write_ast_to_file(filepath, result.ast);
            write_derivations_to_file(filepath, result.derivation);

            {
               using namespace mpark::patterns;
               using namespace munster;

               ast::visitor_variant st_variant{symbol_table_visitor{}};
               result.ast->accept(st_variant);

               auto [has_error, p_root_table] = match(st_variant)(
                  pattern(as<symbol_table_visitor>(arg)) =
                     [&](symbol_table_visitor& vis) -> std::pair<bool, symbol_table*> {
                     print_errors(vis.get_errors(), filepath);

                     return {!std::empty(vis.get_errors()), vis.get_root_table()};
                  });

               ast::visitor_variant ms_variant{memory_size_visitor{p_root_table}};
               result.ast->accept(ms_variant);

               write_symbol_tables_to_file(filepath, p_root_table);

               if (!has_error)
               {
                  ast::visitor_variant cg_variant{code_gen_visitor{p_root_table}};
                  result.ast->accept(cg_variant);

                  match(cg_variant)(
                     pattern(as<code_gen_visitor>(arg)) = [&](code_gen_visitor& vis) {
                        write_moon_code_to_file(filepath, vis.moon_code());
                     });
               }

               /*
               ast::visitor_variant tc_variant{type_checking_visitor{root_table}};
               result.ast->accept(tc_variant);

               match(tc_variant)(
                  pattern(as<type_checking_visitor>(arg)) = [&](type_checking_visitor& vis) {
                     print_errors(vis.get_errors(), filepath);
                  });
                  */
            }
         }
         else
         {
            fmt::print("failed to open file \"{}\". Doublecheck filepath\n", filepath.c_str());

            m_logger.warning(
               "Failed to open file located at \"{}\". Make sure the filepath is valid",
               filepath.c_str());
         }
      }
      else
      {
         fmt::print("file \"{}\" does not have the correct file extension (.src)\n",
                    filepath.c_str());

         m_logger.warning("File \"{}\" does not have the correct file extension (.src)",
                          filepath.c_str());
      }
   }
}

void application::write_lexing_to_file(const std::filesystem::path& path,
                                       const std::span<munster::lex_item>& items) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".m";

   std::ofstream output_file{output_path};
   fmt::print(output_file, "{}", fmt::join(items, "\n"));
}

void application::write_moon_code_to_file(const std::filesystem::path& path,
                                          std::string_view code) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".m";

   std::ofstream output_file{output_path};
   fmt::print(output_file, "{}", code);
}

void application::write_derivations_to_file(const std::filesystem::path& path,
                                            const std::string& derivation) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outderivation";

   std::ofstream output_file{output_path};
   fmt::print(output_file, "{}", derivation);
}

void pre_order_traversal(const munster::ast::node_ptr& node, std::size_t depth,
                         std::ofstream& output_file)
{
   if (!node)
   {
      return;
   }

   const auto name = fmt::format("- {}\n", node);
   fmt::print(output_file, "{:>{}}", name, std::size(name) + depth * 2);
   // fmt::print("{:>{}}", name, std::size(name) + depth * 2);

   for (const auto& child : node->children())
   {
      pre_order_traversal(child, depth + 1, output_file);
   }

   if (std::empty(node->children()))
   {
      pre_order_traversal({}, depth + 1, output_file);
   }
}

void application::write_ast_to_file(const std::filesystem::path& path,
                                    const munster::ast::node_ptr& root) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outast";

   std::ofstream output_file{output_path};

   pre_order_traversal(root, 0, output_file);
}

void st_pre_order_traversal(const munster::symbol_table* root, std::ofstream& output_file)
{
   if (!root)
   {
      return;
   }

   fmt::print(output_file, "{}\n\n", *root);

   for (const auto& symbol : root->symbols() | ranges::views::values)
   {
      st_pre_order_traversal(symbol.link(), output_file);
   }
}

void application::write_symbol_tables_to_file(const std::filesystem::path& path,
                                              const munster::symbol_table* root) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outsymboltables";

   std::ofstream output_file{output_path};

   st_pre_order_traversal(root, output_file);
}

auto application::fancy_lexical_error_type(munster::grammar::token_type value) const -> std::string
{
   // NOLINTNEXTLINE
   assert(munster::grammar::is_token_invalid(value) && "Only invalid types should be passed");

   using namespace munster::grammar;

   if (value == token_type::e_invalid_char)
   {
      return "invalid character";
   }

   if (value == token_type::e_invalid_id)
   {
      return "invalid identifier";
   }

   if (value == token_type::e_invalid_num)
   {
      return "invalid number literal";
   }

   if (value == token_type::e_invalid_str)
   {
      return "invalid string literal";
   }

   return "invalid comment";
}
