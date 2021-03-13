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

#include <toy_compiler/core/parser.hpp>

#include <toy_compiler/grammar/rule.hpp>
#include <toy_compiler/grammar/symbol.hpp>
#include <toy_compiler/grammar/symbol_table.hpp>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/functional/not_fn.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/reverse.hpp>

#include <iostream>

namespace parse
{
   namespace vi = ranges::views;

   auto construct_symbol_table() -> const grammar::symbol_table;
   auto is_comment(const lex::item& item) -> bool;
   auto is_epsilon(const grammar::symbol& s) -> bool { return s == grammar::token_type::epsilon; }

   auto check_follow_sets(grammar::token_type type, const grammar::symbol& top) -> bool
   {
      const auto tail = ranges::find(grammar::sets::follow, top, &grammar::rule::start)->tail();

      return ranges::find(tail, type) != std::end(tail);
   }

   auto contains(const grammar::symbol_array& tail, grammar::token_type type) -> bool
   {
      return ranges::find(tail, type) != std::end(tail);
   }

   auto parse_impl(std::span<const lex::item> items, util::logger_wrapper log) -> result
   {
      static const grammar::symbol_table table = construct_symbol_table();

      std::vector<error> errors;
      std::vector<grammar::symbol> stack;
      stack.push_back(grammar::symbol::stop());
      stack.push_back(grammar::symbol::start());

      auto item_it = std::begin(items);
      while (stack.back() != grammar::symbol::stop())
      {
         const auto& top_symbol = stack.back();

         if (grammar::is_terminal(top_symbol))
         {
            if (!grammar::is_eof(item_it->type) && top_symbol == item_it->type)
            {
               log.info("Parsed token: {}", *item_it);

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

                  const auto token = get<grammar::symbol_type::terminal>(top_symbol);

                  if (token == grammar::token_type::semi_colon)
                  {
                     errors.push_back(error{.type = error_type::missing_terminal,
                                            .token = token,
                                            .line_number = (item_it - 1)->line});
                  }
                  else
                  {
                     errors.push_back(error{.type = error_type::missing_terminal,
                                            .token = token,
                                            .line_number = item_it->line});
                  }

                  while (!grammar::is_eof(item_it->type) && item_it->type != top_symbol)
                  {
                     log.warning("\t{} : {}", top_symbol, *item_it);

                     ++item_it;
                  }
               }
            }
         }
         else
         {
            const auto head = get<grammar::symbol_type::non_terminal>(top_symbol);
            const auto& rule = table.lookup({head, item_it->type});
            if (rule != grammar::rule{})
            {
               const auto symbols = rule.tail() | ranges::views::reverse |
                  vi::filter(ranges::not_fn(is_epsilon)) | ranges::to_vector;

               log.info("Applying derivation: {}", grammar::rule{head, symbols});

               stack.pop_back();

               for (auto& symbol : symbols)
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
                     ranges::find(grammar::sets::first, top_symbol, &grammar::rule::start);
                  const auto& follow_it =
                     ranges::find(grammar::sets::follow, top_symbol, &grammar::rule::start);

                  log.warning("SCANNING OF {}...", top_symbol);

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
      }

      if (!grammar::is_eof(item_it->type) || !std::empty(errors))
      {
         return {.value = status::error, .errors = errors};
      }

      return {.value = status::success, .errors = monad::none};
   }

   auto parse_items(std::span<const lex::item> items, util::logger_wrapper log) -> result
   {
      const auto cleaned = items | vi::filter(ranges::not_fn(is_comment)) | ranges::to_vector;

      return parse_impl(cleaned, log);
   }

   auto construct_symbol_table() -> const grammar::symbol_table
   {
      using namespace grammar;

      symbol_table table{};
      symbol_array epsilon = {token_type::epsilon};

      // <AddOp>
      {
         table.set_rule({grammar_type::add_op, token_type::plus}, {token_type::plus});
         table.set_rule({grammar_type::add_op, token_type::minus}, {token_type::minus});
         table.set_rule({grammar_type::add_op, token_type::or_op}, {token_type::or_op});
      }

      // <aParams>
      {
         symbol_array common{grammar_type::expr, grammar_type::a_params_tail};
         table.set_rule({grammar_type::a_params, token_type::plus}, common);
         table.set_rule({grammar_type::a_params, token_type::minus}, common);
         table.set_rule({grammar_type::a_params, token_type::id}, common);
         table.set_rule({grammar_type::a_params, token_type::integer_lit}, common);
         table.set_rule({grammar_type::a_params, token_type::float_lit}, common);
         table.set_rule({grammar_type::a_params, token_type::str_lit}, common);
         table.set_rule({grammar_type::a_params, token_type::left_paren}, common);
         table.set_rule({grammar_type::a_params, token_type::right_paren}, {token_type::epsilon});
         table.set_rule({grammar_type::a_params, token_type::not_op}, common);
         table.set_rule({grammar_type::a_params, token_type::qmark}, common);
      }

      // <aParamsTail>
      {
         table.set_rule({grammar_type::a_params_tail, token_type::comma},
                        {token_type::comma, grammar_type::expr, grammar_type::a_params_tail});
         table.set_rule({grammar_type::a_params_tail, token_type::right_paren}, epsilon);
      }

      // <ArithExpr>
      {
         symbol_array common{grammar_type::term, grammar_type::arith_expr_tail};
         table.set_rule({grammar_type::arith_expr, token_type::minus}, common);
         table.set_rule({grammar_type::arith_expr, token_type::plus}, common);
         table.set_rule({grammar_type::arith_expr, token_type::id}, common);
         table.set_rule({grammar_type::arith_expr, token_type::integer_lit}, common);
         table.set_rule({grammar_type::arith_expr, token_type::float_lit}, common);
         table.set_rule({grammar_type::arith_expr, token_type::str_lit}, common);
         table.set_rule({grammar_type::arith_expr, token_type::left_paren}, common);
         table.set_rule({grammar_type::arith_expr, token_type::not_op}, common);
         table.set_rule({grammar_type::arith_expr, token_type::qmark}, common);
      }

      // <ArithExprTail>
      {
         symbol_array common{grammar_type::add_op, grammar_type::term,
                             grammar_type::arith_expr_tail};
         table.set_rule({grammar_type::arith_expr_tail, token_type::plus}, common);
         table.set_rule({grammar_type::arith_expr_tail, token_type::minus}, common);
         table.set_rule({grammar_type::arith_expr_tail, token_type::or_op}, common);
         table.set_rule({grammar_type::arith_expr_tail, token_type::comma}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::right_square}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::semi_colon}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::right_paren}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::colon}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::equal}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::not_equal}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::less_than}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::greater_than}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::less_equal_than}, epsilon);
         table.set_rule({grammar_type::arith_expr_tail, token_type::greater_equal_than}, epsilon);
      }

      // <ArraySizeRept>
      {
         table.set_rule({grammar_type::array_size_rept, token_type::comma}, epsilon);
         table.set_rule({grammar_type::array_size_rept, token_type::left_square},
                        {token_type::left_square, grammar_type::int_num, token_type::right_square,
                         grammar_type::array_size_rept});
         table.set_rule({grammar_type::array_size_rept, token_type::semi_colon}, epsilon);
         table.set_rule({grammar_type::array_size_rept, token_type::right_paren}, epsilon);
      }

      // <AssignOp>
      {
         table.set_rule({grammar_type::assign_op, token_type::assign}, {token_type::assign});
      }

      // <AssignStatTail>
      {
         table.set_rule({grammar_type::assign_stat_tail, token_type::assign},
                        {grammar_type::assign_op, grammar_type::expr});
      }

      // <ClassDecl>
      {
         table.set_rule({grammar_type::class_decl, token_type::id_class},
                        {token_type::id_class, token_type::id, grammar_type::inherit,
                         token_type::left_brace, grammar_type::class_decl_body,
                         token_type::right_brace, token_type::semi_colon,
                         grammar_type::class_decl});
         table.set_rule({grammar_type::class_decl, token_type::id_func}, epsilon);
         table.set_rule({grammar_type::class_decl, token_type::id_main}, epsilon);
      }

      // <ClassDeclBody>
      {
         symbol_array common{grammar_type::visibility, grammar_type::member_decl,
                             grammar_type::class_decl_body};

         table.set_rule({grammar_type::class_decl_body, token_type::id}, common);
         table.set_rule({grammar_type::class_decl_body, token_type::right_brace}, epsilon);
         table.set_rule({grammar_type::class_decl_body, token_type::id_func}, common);
         table.set_rule({grammar_type::class_decl_body, token_type::id_integer}, common);
         table.set_rule({grammar_type::class_decl_body, token_type::id_float}, common);
         table.set_rule({grammar_type::class_decl_body, token_type::id_string}, common);
         table.set_rule({grammar_type::class_decl_body, token_type::id_public}, common);
         table.set_rule({grammar_type::class_decl_body, token_type::id_private}, common);
      }

      // <ClassMethod>
      {
         table.set_rule({grammar_type::class_method, token_type::double_colon},
                        {token_type::double_colon, token_type::id});
         table.set_rule({grammar_type::class_method, token_type::left_paren}, epsilon);
      }

      // <Expr>
      {
         symbol_array common{grammar_type::arith_expr, grammar_type::expr_tail};
         table.set_rule({grammar_type::expr, token_type::plus}, common);
         table.set_rule({grammar_type::expr, token_type::minus}, common);
         table.set_rule({grammar_type::expr, token_type::id}, common);
         table.set_rule({grammar_type::expr, token_type::integer_lit}, common);
         table.set_rule({grammar_type::expr, token_type::float_lit}, common);
         table.set_rule({grammar_type::expr, token_type::str_lit}, common);
         table.set_rule({grammar_type::expr, token_type::left_paren}, common);
         table.set_rule({grammar_type::expr, token_type::not_op}, common);
         table.set_rule({grammar_type::expr, token_type::qmark}, common);
      }

      // <ExprTail>
      {
         symbol_array common{grammar_type::relop, grammar_type::arith_expr};

         table.set_rule({grammar_type::expr_tail, token_type::comma}, epsilon);
         table.set_rule({grammar_type::expr_tail, token_type::right_square}, epsilon);
         table.set_rule({grammar_type::expr_tail, token_type::semi_colon}, epsilon);
         table.set_rule({grammar_type::expr_tail, token_type::right_paren}, epsilon);
         table.set_rule({grammar_type::expr_tail, token_type::colon}, epsilon);
         table.set_rule({grammar_type::expr_tail, token_type::equal}, common);
         table.set_rule({grammar_type::expr_tail, token_type::not_equal}, common);
         table.set_rule({grammar_type::expr_tail, token_type::less_than}, common);
         table.set_rule({grammar_type::expr_tail, token_type::greater_than}, common);
         table.set_rule({grammar_type::expr_tail, token_type::less_equal_than}, common);
         table.set_rule({grammar_type::expr_tail, token_type::greater_equal_than}, common);
      }

      // <Factor>
      {
         table.set_rule({grammar_type::factor, token_type::minus},
                        {grammar_type::sign, grammar_type::factor});
         table.set_rule({grammar_type::factor, token_type::plus},
                        {grammar_type::sign, grammar_type::factor});
         table.set_rule({grammar_type::factor, token_type::id}, {grammar_type::func_or_var});
         table.set_rule({grammar_type::factor, token_type::float_lit}, {token_type::float_lit});
         table.set_rule({grammar_type::factor, token_type::integer_lit}, {token_type::integer_lit});
         table.set_rule({grammar_type::factor, token_type::str_lit}, {token_type::str_lit});
         table.set_rule({grammar_type::factor, token_type::left_paren},
                        {token_type::left_paren, grammar_type::expr, token_type::right_paren});
         table.set_rule({grammar_type::factor, token_type::not_op},
                        {token_type::not_op, grammar_type::factor});
         table.set_rule({grammar_type::factor, token_type::qmark},
                        {token_type::qmark, token_type::left_square, grammar_type::expr,
                         token_type::colon, grammar_type::expr, token_type::colon,
                         grammar_type::expr, token_type::right_square});
      }

      // <fParams>
      {
         symbol_array common{grammar_type::type, token_type::id, grammar_type::array_size_rept,
                             grammar_type::f_params_tail};
         table.set_rule({grammar_type::f_params, token_type::id}, common);
         table.set_rule({grammar_type::f_params, token_type::right_paren}, epsilon);
         table.set_rule({grammar_type::f_params, token_type::id_integer}, common);
         table.set_rule({grammar_type::f_params, token_type::id_float}, common);
         table.set_rule({grammar_type::f_params, token_type::id_string}, common);
      }

      // <fParamsTail>
      {
         table.set_rule({grammar_type::f_params_tail, token_type::comma},
                        {token_type::comma, grammar_type::type, token_type::id,
                         grammar_type::array_size_rept, grammar_type::f_params_tail});
         table.set_rule({grammar_type::f_params_tail, token_type::right_paren}, epsilon);
      }

      // <FuncBody>
      {
         table.set_rule({grammar_type::func_body, token_type::left_brace},
                        {token_type::left_brace, grammar_type::method_body_var,
                         grammar_type::statement_list, token_type::right_brace});
      }

      {
         table.set_rule({grammar_type::func_decl, token_type::id_func},
                        {token_type::id_func, token_type::id, token_type::left_paren,
                         grammar_type::f_params, token_type::right_paren, token_type::colon,
                         grammar_type::func_decl_tail, token_type::semi_colon});
      }

      // <FuncDeclTail>
      {
         table.set_rule({grammar_type::func_decl_tail, token_type::id}, {grammar_type::type});
         table.set_rule({grammar_type::func_decl_tail, token_type::id_void}, {token_type::id_void});
         table.set_rule({grammar_type::func_decl_tail, token_type::id_integer},
                        {grammar_type::type});
         table.set_rule({grammar_type::func_decl_tail, token_type::id_float}, {grammar_type::type});
         table.set_rule({grammar_type::func_decl_tail, token_type::id_string},
                        {grammar_type::type});
      }

      // <FuncDef>
      {
         table.set_rule({grammar_type::func_def, token_type::id_func},
                        {grammar_type::function, grammar_type::func_def});
         table.set_rule({grammar_type::func_def, token_type::id_main}, epsilon);
      }

      // <FuncHead>
      {
         table.set_rule({grammar_type::func_head, token_type::id_func},
                        {token_type::id_func, token_type::id, grammar_type::class_method,
                         token_type::left_paren, grammar_type::f_params, token_type::right_paren,
                         token_type::colon, grammar_type::func_decl_tail});
      }

      // <FuncOrAssignStat>
      {
         table.set_rule({grammar_type::func_or_assign_stat, token_type::id},
                        {token_type::id, grammar_type::func_or_assign_stat_idnest});
      }

      // <FuncOrAssignStatIdnest>
      {
         symbol_array common{grammar_type::indice_rep,
                             grammar_type::func_or_assign_stat_idnest_var_tail};

         table.set_rule({grammar_type::func_or_assign_stat_idnest, token_type::left_square},
                        common);
         table.set_rule({grammar_type::func_or_assign_stat_idnest, token_type::assign}, common);
         table.set_rule({grammar_type::func_or_assign_stat_idnest, token_type::left_paren},
                        {token_type::left_paren, grammar_type::a_params, token_type::right_paren,
                         grammar_type::func_or_assign_stat_idnest_func_tail});
         table.set_rule({grammar_type::func_or_assign_stat_idnest, token_type::period}, common);
      }

      // <FuncOrAssignStatIdnestFuncTail>
      {
         table.set_rule(
            {grammar_type::func_or_assign_stat_idnest_func_tail, token_type::semi_colon}, epsilon);
         table.set_rule({grammar_type::func_or_assign_stat_idnest_func_tail, token_type::period},
                        {token_type::period, token_type::id, grammar_type::func_stat_tail});
      }

      // <FuncOrAssignStatIdnestVarTail>
      {
         table.set_rule({grammar_type::func_or_assign_stat_idnest_var_tail, token_type::assign},
                        {grammar_type::assign_stat_tail});
         table.set_rule(
            {grammar_type::func_or_assign_stat_idnest_var_tail, token_type::period},
            {token_type::period, token_type::id, grammar_type::func_or_assign_stat_idnest});
      }

      // <FuncOrVar>
      {
         table.set_rule({grammar_type::func_or_var, token_type::id},
                        {token_type::id, grammar_type::func_or_var_idnest});
      }

      // <FuncOrVarIdnest>
      {
         symbol_array common{grammar_type::indice_rep, grammar_type::func_or_var_idnest_tail};

         table.set_rule({grammar_type::func_or_var_idnest, token_type::plus}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::minus}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::or_op}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::comma}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::left_square}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::right_square}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::semi_colon}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::left_paren},
                        {token_type::left_paren, grammar_type::a_params, token_type::right_paren,
                         grammar_type::func_or_var_idnest_tail});
         table.set_rule({grammar_type::func_or_var_idnest, token_type::right_paren}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::colon}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::period}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::mult}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::div}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::and_op}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::equal}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::not_equal}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::less_than}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::greater_than}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::less_equal_than}, common);
         table.set_rule({grammar_type::func_or_var_idnest, token_type::greater_equal_than}, common);
      }

      // <FuncOrVarIdnestTail>
      {
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::plus}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::minus}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::or_op}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::comma}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::right_square}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::semi_colon}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::right_paren}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::colon}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::period},
                        {token_type::period, token_type::id, grammar_type::func_or_var_idnest});
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::mult}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::div}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::and_op}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::equal}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::not_equal}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::less_than}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::greater_than}, epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::less_equal_than},
                        epsilon);
         table.set_rule({grammar_type::func_or_var_idnest_tail, token_type::greater_equal_than},
                        epsilon);
      }

      // <FuncStatTail>
      {
         table.set_rule({grammar_type::func_stat_tail, token_type::left_square},
                        {grammar_type::indice_rep, token_type::period, token_type::id,
                         grammar_type::func_stat_tail});
         table.set_rule({grammar_type::func_stat_tail, token_type::left_paren},
                        {token_type::left_paren, grammar_type::a_params, token_type::right_paren,
                         grammar_type::func_stat_tail_idnest});
         table.set_rule({grammar_type::func_stat_tail, token_type::period},
                        {grammar_type::indice_rep, token_type::period, token_type::id,
                         grammar_type::func_stat_tail});
      }

      // <FuncStatTailIdnest>
      {
         table.set_rule({grammar_type::func_stat_tail_idnest, token_type::semi_colon}, epsilon);
         table.set_rule({grammar_type::func_stat_tail_idnest, token_type::period},
                        {token_type::period, token_type::id, grammar_type::func_stat_tail});
      }

      // <Function>
      {
         table.set_rule({grammar_type::function, token_type::id_func},
                        {grammar_type::func_head, grammar_type::func_body});
      }

      // <IndiceRep>
      {
         table.set_rule({grammar_type::indice_rep, token_type::plus}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::minus}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::or_op}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::comma}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::left_square},
                        {token_type::left_square, grammar_type::expr, token_type::right_square,
                         grammar_type::indice_rep});
         table.set_rule({grammar_type::indice_rep, token_type::right_square}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::assign}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::semi_colon}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::right_paren}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::colon}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::period}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::mult}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::div}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::and_op}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::equal}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::not_equal}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::less_than}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::greater_than}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::less_equal_than}, epsilon);
         table.set_rule({grammar_type::indice_rep, token_type::greater_equal_than}, epsilon);
      }

      // <Inherits>
      {
         table.set_rule({grammar_type::inherit, token_type::left_brace}, epsilon);
         table.set_rule({grammar_type::inherit, token_type::id_inherits},
                        {token_type::id_inherits, token_type::id, grammar_type::nested_id});
      }

      // <IntNum>
      {
         table.set_rule({grammar_type::int_num, token_type::integer_lit},
                        {token_type::integer_lit});
         table.set_rule({grammar_type::int_num, token_type::right_square}, epsilon);
      }

      //////////////////// HERE

      // <MemberDecl>
      {
         table.set_rule({grammar_type::member_decl, token_type::id}, {grammar_type::var_decl});
         table.set_rule({grammar_type::member_decl, token_type::id_string},
                        {grammar_type::var_decl});
         table.set_rule({grammar_type::member_decl, token_type::id_float},
                        {grammar_type::var_decl});
         table.set_rule({grammar_type::member_decl, token_type::id_integer},
                        {grammar_type::var_decl});
         table.set_rule({grammar_type::member_decl, token_type::id_func},
                        {grammar_type::func_decl});
      }

      // <MethodBodyVar>
      {
         table.set_rule({grammar_type::method_body_var, token_type::id}, epsilon);
         table.set_rule({grammar_type::method_body_var, token_type::id_continue}, epsilon);
         table.set_rule({grammar_type::method_body_var, token_type::id_break}, epsilon);
         table.set_rule({grammar_type::method_body_var, token_type::id_return}, epsilon);
         table.set_rule({grammar_type::method_body_var, token_type::id_write}, epsilon);
         table.set_rule({grammar_type::method_body_var, token_type::id_read}, epsilon);
         table.set_rule({grammar_type::method_body_var, token_type::id_while}, epsilon);
         table.set_rule({grammar_type::method_body_var, token_type::id_if}, epsilon);
         table.set_rule({grammar_type::method_body_var, token_type::id_var},
                        {token_type::id_var, token_type::left_brace, grammar_type::var_decl_rep,
                         token_type::right_brace});
         table.set_rule({grammar_type::method_body_var, token_type::right_brace}, epsilon);
      }

      // <MultOp>
      {
         table.set_rule({grammar_type::mult_op, token_type::and_op}, {token_type::and_op});
         table.set_rule({grammar_type::mult_op, token_type::div}, {token_type::div});
         table.set_rule({grammar_type::mult_op, token_type::mult}, {token_type::mult});
      }

      // <NestedId>
      {
         table.set_rule({grammar_type::nested_id, token_type::left_brace}, epsilon);
         table.set_rule({grammar_type::nested_id, token_type::comma},
                        {token_type::comma, token_type::id, grammar_type::nested_id});
      }

      // <Prog>
      {
         symbol_array common{grammar_type::class_decl, grammar_type::func_def, token_type::id_main,
                             grammar_type::func_body};
         table.set_rule({grammar_type::prog, token_type::id_main}, common);
         table.set_rule({grammar_type::prog, token_type::id_func}, common);
         table.set_rule({grammar_type::prog, token_type::id_class}, common);
      }

      // <RelOp>
      {
         table.set_rule({grammar_type::relop, token_type::greater_equal_than},
                        {token_type::greater_equal_than});
         table.set_rule({grammar_type::relop, token_type::less_equal_than},
                        {token_type::less_equal_than});
         table.set_rule({grammar_type::relop, token_type::greater_than},
                        {token_type::greater_than});
         table.set_rule({grammar_type::relop, token_type::less_than}, {token_type::less_than});
         table.set_rule({grammar_type::relop, token_type::not_equal}, {token_type::not_equal});
         table.set_rule({grammar_type::relop, token_type::equal}, {token_type::equal});
      }

      // <Sign>
      {
         table.set_rule({grammar_type::sign, token_type::plus}, {token_type::plus});
         table.set_rule({grammar_type::sign, token_type::minus}, {token_type::minus});
      }

      // <Start>
      {
         table.set_rule({grammar_type::start, token_type::id_main}, {grammar_type::prog});
         table.set_rule({grammar_type::start, token_type::id_func}, {grammar_type::prog});
         table.set_rule({grammar_type::start, token_type::id_class}, {grammar_type::prog});
      }

      // <StatBlock>
      {
         symbol_array common{grammar_type::statement};
         table.set_rule({grammar_type::stat_block, token_type::id}, common);
         table.set_rule({grammar_type::stat_block, token_type::semi_colon}, epsilon);
         table.set_rule({grammar_type::stat_block, token_type::id_continue}, common);
         table.set_rule({grammar_type::stat_block, token_type::id_break}, common);
         table.set_rule({grammar_type::stat_block, token_type::id_return}, common);
         table.set_rule({grammar_type::stat_block, token_type::id_write}, common);
         table.set_rule({grammar_type::stat_block, token_type::id_read}, common);
         table.set_rule({grammar_type::stat_block, token_type::id_while}, common);
         table.set_rule({grammar_type::stat_block, token_type::id_else}, epsilon);
         table.set_rule({grammar_type::stat_block, token_type::id_if}, common);
         table.set_rule(
            {grammar_type::stat_block, token_type::left_brace},
            {token_type::left_brace, grammar_type::statement_list, token_type::right_brace});
      }

      ///////////////////// HERE

      // <Statement>
      {
         table.set_rule({grammar_type::statement, token_type::id},
                        {grammar_type::func_or_assign_stat, token_type::semi_colon});
         table.set_rule({grammar_type::statement, token_type::id_if},
                        {token_type::id_if, token_type::left_paren, grammar_type::expr,
                         token_type::right_paren, token_type::id_then, grammar_type::stat_block,
                         token_type::id_else, grammar_type::stat_block, token_type::semi_colon});
         table.set_rule({grammar_type::statement, token_type::id_while},
                        {token_type::id_while, token_type::left_paren, grammar_type::expr,
                         token_type::right_paren, grammar_type::stat_block,
                         token_type::semi_colon});
         table.set_rule({grammar_type::statement, token_type::id_read},
                        {token_type::id_read, token_type::left_paren, grammar_type::variable,
                         token_type::right_paren, token_type::semi_colon});
         table.set_rule({grammar_type::statement, token_type::id_write},
                        {token_type::id_write, token_type::left_paren, grammar_type::expr,
                         token_type::right_paren, token_type::semi_colon});
         table.set_rule({grammar_type::statement, token_type::id_return},
                        {token_type::id_return, token_type::left_paren, grammar_type::expr,
                         token_type::right_paren, token_type::semi_colon});
         table.set_rule({grammar_type::statement, token_type::id_break},
                        {token_type::id_break, token_type::semi_colon});
         table.set_rule({grammar_type::statement, token_type::id_continue},
                        {token_type::id_continue, token_type::semi_colon});
      }

      // <StatementList>
      {
         symbol_array common{grammar_type::statement, grammar_type::statement_list};
         table.set_rule({grammar_type::statement_list, token_type::id}, common);
         table.set_rule({grammar_type::statement_list, token_type::right_brace}, epsilon);
         table.set_rule({grammar_type::statement_list, token_type::id_if}, common);
         table.set_rule({grammar_type::statement_list, token_type::id_while}, common);
         table.set_rule({grammar_type::statement_list, token_type::id_read}, common);
         table.set_rule({grammar_type::statement_list, token_type::id_write}, common);
         table.set_rule({grammar_type::statement_list, token_type::id_return}, common);
         table.set_rule({grammar_type::statement_list, token_type::id_break}, common);
         table.set_rule({grammar_type::statement_list, token_type::id_continue}, common);
      }

      // <Term>
      {
         symbol_array common{grammar_type::factor, grammar_type::term_tail};
         table.set_rule({grammar_type::term, token_type::plus}, common);
         table.set_rule({grammar_type::term, token_type::minus}, common);
         table.set_rule({grammar_type::term, token_type::id}, common);
         table.set_rule({grammar_type::term, token_type::integer_lit}, common);
         table.set_rule({grammar_type::term, token_type::float_lit}, common);
         table.set_rule({grammar_type::term, token_type::str_lit}, common);
         table.set_rule({grammar_type::term, token_type::left_paren}, common);
         table.set_rule({grammar_type::term, token_type::not_op}, common);
         table.set_rule({grammar_type::term, token_type::qmark}, common);
      }

      // <TermTail>
      {
         symbol_array common{grammar_type::mult_op, grammar_type::factor, grammar_type::term_tail};
         table.set_rule({grammar_type::term_tail, token_type::plus}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::minus}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::or_op}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::comma}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::right_square}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::semi_colon}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::right_paren}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::colon}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::mult}, common);
         table.set_rule({grammar_type::term_tail, token_type::div}, common);
         table.set_rule({grammar_type::term_tail, token_type::and_op}, common);
         table.set_rule({grammar_type::term_tail, token_type::equal}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::not_equal}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::less_than}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::greater_than}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::less_equal_than}, epsilon);
         table.set_rule({grammar_type::term_tail, token_type::greater_equal_than}, epsilon);
      }

      // <Type>
      {
         table.set_rule({grammar_type::type, token_type::id}, {token_type::id});
         table.set_rule({grammar_type::type, token_type::id_integer}, {token_type::id_integer});
         table.set_rule({grammar_type::type, token_type::id_float}, {token_type::id_float});
         table.set_rule({grammar_type::type, token_type::id_string}, {token_type::id_string});
      }

      // <VarDecl>
      {
         symbol_array common{grammar_type::type, token_type::id, grammar_type::array_size_rept,
                             token_type::semi_colon};
         table.set_rule({grammar_type::var_decl, token_type::id}, common);
         table.set_rule({grammar_type::var_decl, token_type::id_integer}, common);
         table.set_rule({grammar_type::var_decl, token_type::id_float}, common);
         table.set_rule({grammar_type::var_decl, token_type::id_string}, common);
      }

      // <VarDeclRep>
      {
         symbol_array common{grammar_type::var_decl, grammar_type::var_decl_rep};
         table.set_rule({grammar_type::var_decl_rep, token_type::id}, common);
         table.set_rule({grammar_type::var_decl_rep, token_type::right_brace}, epsilon);
         table.set_rule({grammar_type::var_decl_rep, token_type::id_integer}, common);
         table.set_rule({grammar_type::var_decl_rep, token_type::id_float}, common);
         table.set_rule({grammar_type::var_decl_rep, token_type::id_string}, common);
      }

      // <Variable>
      {
         table.set_rule({grammar_type::variable, token_type::id},
                        {token_type::id, grammar_type::variable_idnest});
      }

      // <VariableIdnest>
      {
         symbol_array common{grammar_type::indice_rep, grammar_type::variable_idnest_tail};
         table.set_rule({grammar_type::variable_idnest, token_type::left_square}, common);
         table.set_rule({grammar_type::variable_idnest, token_type::right_paren}, common);
         table.set_rule({grammar_type::variable_idnest, token_type::period}, common);
      }

      // <VariableIdnestTail>
      {
         table.set_rule({grammar_type::variable_idnest_tail, token_type::right_paren}, epsilon);
         table.set_rule({grammar_type::variable_idnest_tail, token_type::period},
                        {token_type::period, token_type::id, grammar_type::variable_idnest});
      }

      // <Visibility>
      {
         table.set_rule({grammar_type::visibility, token_type::id}, epsilon);
         table.set_rule({grammar_type::visibility, token_type::id_func}, epsilon);
         table.set_rule({grammar_type::visibility, token_type::id_integer}, epsilon);
         table.set_rule({grammar_type::visibility, token_type::id_float}, epsilon);
         table.set_rule({grammar_type::visibility, token_type::id_string}, epsilon);
         table.set_rule({grammar_type::visibility, token_type::id_public}, {token_type::id_public});
         table.set_rule({grammar_type::visibility, token_type::id_private},
                        {token_type::id_private});
      }

      return table;
   }

   auto is_comment(const lex::item& item) -> bool
   {
      using namespace grammar;
      return (item.type == token_type::block_cmt) || (item.type == token_type::line_cmt);
   }
} // namespace parse
