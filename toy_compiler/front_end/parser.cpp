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

#include <toy_compiler/front_end/parser.hpp>

#include <toy_compiler/front_end/ast/node.hpp>
#include <toy_compiler/front_end/grammar/production.hpp>
#include <toy_compiler/front_end/grammar/production_table.hpp>
#include <toy_compiler/front_end/grammar/symbol.hpp>

#include <fmt/color.h>
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

namespace fr
{
   namespace vi = ranges::views;

   auto construct_symbol_table() -> const grammar::production_table;
   auto is_comment(const fr::lex_item& item) -> bool;
   auto is_epsilon(const grammar::symbol& s) -> bool { return s == grammar::token_type::epsilon; }
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

   auto skip_error() {}

   auto parse_impl(std::span<const fr::lex_item> items, util::logger_wrapper log) -> parse_result
   {
      static const grammar::production_table table = construct_symbol_table();

      std::vector<parse_error> errors;
      std::vector<ast::node_ptr> nodes;
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

                  const auto token = get<grammar::symbol_type::terminal>(top_symbol);
                  if (token == grammar::token_type::semi_colon)
                  {
                     errors.push_back(parse_error{.type = parse_error_type::missing_terminal,
                                                  .token = token,
                                                  .pos = (item_it - 1)->pos});
                  }
                  else
                  {
                     errors.push_back(parse_error{.type = parse_error_type::missing_terminal,
                                                  .token = token,
                                                  .pos = item_it->pos});
                  }

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
               nodes.push_back(ast::node_factory(type, *(item_it - 1), nodes));
            }
            else
            {
               nodes.push_back(ast::node_factory(type, {}, nodes));
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
              .errors = monad::none};
   }

   auto parse_items(std::span<const fr::lex_item> items, util::logger_wrapper log) -> parse_result
   {
      const auto cleaned = items | vi::filter(ranges::not_fn(is_comment)) | ranges::to_vector;

      return parse_impl(cleaned, log);
   }

   auto construct_symbol_table() -> const grammar::production_table
   {
      using namespace grammar;

      production_table table{};
      symbol_array epsilon = {token_type::epsilon, sem::action_type::epsilon};

      // <AddOp>
      {
         table.set_production({sem::grammar_type::add_op, token_type::plus},
                              {token_type::plus, sem::action_type::value});
         table.set_production({sem::grammar_type::add_op, token_type::minus},
                              {token_type::minus, sem::action_type::value});
         table.set_production({sem::grammar_type::add_op, token_type::or_op},
                              {token_type::or_op, sem::action_type::value});
      }

      // <aParams>
      {
         symbol_array common{sem::grammar_type::expr, sem::grammar_type::a_params_tail};
         table.set_production({sem::grammar_type::a_params, token_type::plus}, common);
         table.set_production({sem::grammar_type::a_params, token_type::minus}, common);
         table.set_production({sem::grammar_type::a_params, token_type::id}, common);
         table.set_production({sem::grammar_type::a_params, token_type::integer_lit}, common);
         table.set_production({sem::grammar_type::a_params, token_type::float_lit}, common);
         table.set_production({sem::grammar_type::a_params, token_type::str_lit}, common);
         table.set_production({sem::grammar_type::a_params, token_type::left_paren}, common);
         table.set_production({sem::grammar_type::a_params, token_type::right_paren},
                              {token_type::epsilon});
         table.set_production({sem::grammar_type::a_params, token_type::not_op}, common);
         table.set_production({sem::grammar_type::a_params, token_type::qmark}, common);
      }

      // <aParamsTail>
      {
         table.set_production(
            {sem::grammar_type::a_params_tail, token_type::comma},
            {token_type::comma, sem::grammar_type::expr, sem::grammar_type::a_params_tail});
         table.set_production({sem::grammar_type::a_params_tail, token_type::right_paren}, epsilon);
      }

      // <ArithExpr>
      {
         symbol_array common{sem::grammar_type::term, sem::action_type::term_expr,
                             sem::grammar_type::arith_expr_tail, sem::action_type::arith_expr_list};
         table.set_production({sem::grammar_type::arith_expr, token_type::minus}, common);
         table.set_production({sem::grammar_type::arith_expr, token_type::plus}, common);
         table.set_production({sem::grammar_type::arith_expr, token_type::id}, common);
         table.set_production({sem::grammar_type::arith_expr, token_type::integer_lit}, common);
         table.set_production({sem::grammar_type::arith_expr, token_type::float_lit}, common);
         table.set_production({sem::grammar_type::arith_expr, token_type::str_lit}, common);
         table.set_production({sem::grammar_type::arith_expr, token_type::left_paren}, common);
         table.set_production({sem::grammar_type::arith_expr, token_type::not_op}, common);
         table.set_production({sem::grammar_type::arith_expr, token_type::qmark}, common);
      }

      // <ArithExprTail>
      {
         symbol_array common{sem::grammar_type::add_op,         sem::action_type::add_op,
                             sem::grammar_type::term,           sem::action_type::term_expr,
                             sem::action_type::arith_tail_expr, sem::grammar_type::arith_expr_tail};
         const auto key = sem::grammar_type::arith_expr_tail;
         table.set_production({key, token_type::plus}, common);
         table.set_production({key, token_type::minus}, common);
         table.set_production({key, token_type::or_op}, common);
         table.set_production({key, token_type::comma}, epsilon);
         table.set_production({key, token_type::right_square}, epsilon);
         table.set_production({key, token_type::semi_colon}, epsilon);
         table.set_production({key, token_type::right_paren}, epsilon);
         table.set_production({key, token_type::colon}, epsilon);
         table.set_production({key, token_type::equal}, epsilon);
         table.set_production({key, token_type::not_equal}, epsilon);
         table.set_production({key, token_type::less_than}, epsilon);
         table.set_production({key, token_type::greater_than}, epsilon);
         table.set_production({key, token_type::less_equal_than}, epsilon);
         table.set_production({key, token_type::greater_equal_than}, epsilon);
      }

      // <ArraySizeRept>
      {
         const auto key = sem::grammar_type::array_size_rept;
         table.set_production({key, token_type::comma}, epsilon);
         table.set_production({key, token_type::left_square},
                              {token_type::left_square, sem::grammar_type::int_num,
                               sem::action_type::array_size, token_type::right_square,
                               sem::grammar_type::array_size_rept});
         table.set_production({key, token_type::semi_colon}, epsilon);
         table.set_production({key, token_type::right_paren}, epsilon);
      }

      // <AssignOp>
      {
         table.set_production({sem::grammar_type::assign_op, token_type::assign},
                              {token_type::assign});
      }

      // <AssignStatTail>
      {
         table.set_production({sem::grammar_type::assign_stat_tail, token_type::assign},
                              {sem::grammar_type::assign_op, sem::grammar_type::expr});
      }

      // <ClassDecl>
      {
         table.set_production({sem::grammar_type::class_decl, token_type::id_class},
                              {token_type::id_class, token_type::id, sem::action_type::value,
                               sem::grammar_type::inherit, sem::action_type::inheritance_decl_list,
                               token_type::left_brace, sem::grammar_type::class_decl_body,
                               sem::action_type::class_body_decl_list, token_type::right_brace,
                               token_type::semi_colon, sem::action_type::class_decl,
                               sem::grammar_type::class_decl});
         table.set_production({sem::grammar_type::class_decl, token_type::id_func}, epsilon);
         table.set_production({sem::grammar_type::class_decl, token_type::id_main}, epsilon);
      }

      // <ClassDeclBody>
      {
         symbol_array common{sem::grammar_type::visibility,     sem::action_type::visibility_decl,
                             sem::grammar_type::member_decl,    sem::action_type::member_decl,
                             sem::action_type::class_body_decl, sem::grammar_type::class_decl_body};

         table.set_production({sem::grammar_type::class_decl_body, token_type::id}, common);
         table.set_production({sem::grammar_type::class_decl_body, token_type::right_brace},
                              epsilon);
         table.set_production({sem::grammar_type::class_decl_body, token_type::id_func}, common);
         table.set_production({sem::grammar_type::class_decl_body, token_type::id_integer}, common);
         table.set_production({sem::grammar_type::class_decl_body, token_type::id_float}, common);
         table.set_production({sem::grammar_type::class_decl_body, token_type::id_string}, common);
         table.set_production({sem::grammar_type::class_decl_body, token_type::id_public}, common);
         table.set_production({sem::grammar_type::class_decl_body, token_type::id_private}, common);
      }

      // <ClassMethod>
      {
         table.set_production({sem::grammar_type::class_method, token_type::double_colon},
                              {token_type::double_colon, token_type::id, sem::action_type::value});
         table.set_production({sem::grammar_type::class_method, token_type::left_paren}, epsilon);
      }

      // <Expr>
      {
         symbol_array common{sem::grammar_type::arith_expr, sem::action_type::arith_expr,
                             sem::grammar_type::expr_tail};
         const auto key = sem::grammar_type::expr;
         table.set_production({key, token_type::plus}, common);
         table.set_production({key, token_type::minus}, common);
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::integer_lit}, common);
         table.set_production({key, token_type::float_lit}, common);
         table.set_production({key, token_type::str_lit}, common);
         table.set_production({key, token_type::left_paren}, common);
         table.set_production({key, token_type::not_op}, common);
         table.set_production({key, token_type::qmark}, common);
      }

      // <ExprTail>
      {
         symbol_array common{sem::grammar_type::relop, sem::action_type::rel_op,
                             sem::grammar_type::arith_expr, sem::action_type::arith_expr};
         const auto key = sem::grammar_type::expr_tail;
         table.set_production({key, token_type::comma}, epsilon);
         table.set_production({key, token_type::right_square}, epsilon);
         table.set_production({key, token_type::semi_colon}, epsilon);
         table.set_production({key, token_type::right_paren}, epsilon);
         table.set_production({key, token_type::colon}, epsilon);
         table.set_production({key, token_type::equal}, common);
         table.set_production({key, token_type::not_equal}, common);
         table.set_production({key, token_type::less_than}, common);
         table.set_production({key, token_type::greater_than}, common);
         table.set_production({key, token_type::less_equal_than}, common);
         table.set_production({key, token_type::greater_equal_than}, common);
      }

      // <Factor>
      {
         const auto key = sem::grammar_type::factor;
         table.set_production({key, token_type::minus},
                              {sem::grammar_type::sign, sem::grammar_type::factor});
         table.set_production({key, token_type::plus},
                              {sem::grammar_type::sign, sem::grammar_type::factor});
         table.set_production({key, token_type::id}, {sem::grammar_type::func_or_var});
         table.set_production({key, token_type::float_lit},
                              {token_type::float_lit, sem::action_type::value});
         table.set_production({key, token_type::integer_lit},
                              {token_type::integer_lit, sem::action_type::value});
         table.set_production({key, token_type::str_lit},
                              {token_type::str_lit, sem::action_type::value});
         table.set_production(
            {key, token_type::left_paren},
            {token_type::left_paren, sem::grammar_type::expr, token_type::right_paren});
         table.set_production({key, token_type::not_op},
                              {token_type::not_op, sem::grammar_type::factor});
         table.set_production({key, token_type::qmark},
                              {token_type::qmark, token_type::left_square, sem::grammar_type::expr,
                               token_type::colon, sem::grammar_type::expr, token_type::colon,
                               sem::grammar_type::expr, token_type::right_square});
      }

      // <fParams>
      {
         symbol_array common{sem::grammar_type::type,
                             sem::action_type::type,
                             token_type::id,
                             sem::action_type::value,
                             sem::grammar_type::array_size_rept,
                             sem::action_type::array_size_stmt_list,
                             sem::action_type::function_param_decl,
                             sem::grammar_type::f_params_tail};
         table.set_production({sem::grammar_type::f_params, token_type::id}, common);
         table.set_production({sem::grammar_type::f_params, token_type::right_paren}, epsilon);
         table.set_production({sem::grammar_type::f_params, token_type::id_integer}, common);
         table.set_production({sem::grammar_type::f_params, token_type::id_float}, common);
         table.set_production({sem::grammar_type::f_params, token_type::id_string}, common);
      }

      // <fParamsTail>
      {
         table.set_production(
            {sem::grammar_type::f_params_tail, token_type::comma},
            {token_type::comma, sem::grammar_type::type, sem::action_type::type, token_type::id,
             sem::action_type::value, sem::grammar_type::array_size_rept,
             sem::action_type::array_size_stmt_list, sem::action_type::function_param_decl,
             sem::grammar_type::f_params_tail});
         table.set_production({sem::grammar_type::f_params_tail, token_type::right_paren}, epsilon);
      }

      // <FuncBody>
      {
         table.set_production({sem::grammar_type::func_body, token_type::left_brace},
                              {token_type::left_brace, sem::grammar_type::method_body_var,
                               sem::action_type::method_body_var, sem::grammar_type::statement_list,
                               sem::action_type::statement_list, token_type::right_brace});
      }

      {
         table.set_production({sem::grammar_type::func_decl, token_type::id_func},
                              {token_type::id_func, token_type::id, sem::action_type::value,
                               token_type::left_paren, sem::grammar_type::f_params,
                               sem::action_type::function_param_decl_list, token_type::right_paren,
                               token_type::colon, sem::grammar_type::func_decl_tail,
                               sem::action_type::return_type_decl, token_type::semi_colon});
      }

      // <FuncDeclTail>
      {
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id},
                              {sem::grammar_type::type, sem::action_type::type});
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id_void},
                              {token_type::id_void, sem::action_type::type});
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id_integer},
                              {sem::grammar_type::type, sem::action_type::type});
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id_float},
                              {sem::grammar_type::type, sem::action_type::type});
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id_string},
                              {sem::grammar_type::type, sem::action_type::type});
      }

      // <FuncDef>
      {
         table.set_production(
            {sem::grammar_type::func_def, token_type::id_func},
            {sem::grammar_type::function, sem::action_type::function, sem::grammar_type::func_def});
         table.set_production({sem::grammar_type::func_def, token_type::id_main}, epsilon);
      }

      // <FuncHead>
      {
         table.set_production({sem::grammar_type::func_head, token_type::id_func},
                              {token_type::id_func, token_type::id, sem::action_type::value,
                               sem::grammar_type::class_method, sem::action_type::class_method,
                               token_type::left_paren, sem::grammar_type::f_params,
                               sem::action_type::function_param_decl_list, token_type::right_paren,
                               token_type::colon, sem::grammar_type::func_decl_tail,
                               sem::action_type::return_type_decl});
      }

      // <FuncOrAssignStat>
      {
         table.set_production({sem::grammar_type::func_or_assign_stat, token_type::id},
                              {token_type::id, sem::grammar_type::func_or_assign_stat_idnest});
      }

      // <FuncOrAssignStatIdnest>
      {
         symbol_array common{sem::grammar_type::indice_rep,
                             sem::grammar_type::func_or_assign_stat_idnest_var_tail};

         table.set_production(
            {sem::grammar_type::func_or_assign_stat_idnest, token_type::left_square}, common);
         table.set_production({sem::grammar_type::func_or_assign_stat_idnest, token_type::assign},
                              common);
         table.set_production(
            {sem::grammar_type::func_or_assign_stat_idnest, token_type::left_paren},
            {token_type::left_paren, sem::grammar_type::a_params, token_type::right_paren,
             sem::grammar_type::func_or_assign_stat_idnest_func_tail});
         table.set_production({sem::grammar_type::func_or_assign_stat_idnest, token_type::period},
                              common);
      }

      // <FuncOrAssignStatIdnestFuncTail>
      {
         table.set_production(
            {sem::grammar_type::func_or_assign_stat_idnest_func_tail, token_type::semi_colon},
            epsilon);
         table.set_production(
            {sem::grammar_type::func_or_assign_stat_idnest_func_tail, token_type::period},
            {token_type::period, token_type::id, sem::grammar_type::func_stat_tail});
      }

      // <FuncOrAssignStatIdnestVarTail>
      {
         table.set_production(
            {sem::grammar_type::func_or_assign_stat_idnest_var_tail, token_type::assign},
            {sem::grammar_type::assign_stat_tail});
         table.set_production(
            {sem::grammar_type::func_or_assign_stat_idnest_var_tail, token_type::period},
            {token_type::period, token_type::id, sem::grammar_type::func_or_assign_stat_idnest});
      }

      // <FuncOrVar>
      {
         table.set_production({sem::grammar_type::func_or_var, token_type::id},
                              {token_type::id, sem::grammar_type::func_or_var_idnest});
      }

      // <FuncOrVarIdnest>
      {
         symbol_array common{sem::grammar_type::indice_rep,
                             sem::grammar_type::func_or_var_idnest_tail};

         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::plus}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::minus}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::or_op}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::comma}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::left_square},
                              common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::right_square},
                              common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::semi_colon},
                              common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::left_paren},
                              {token_type::left_paren, sem::grammar_type::a_params,
                               token_type::right_paren,
                               sem::grammar_type::func_or_var_idnest_tail});
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::right_paren},
                              common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::colon}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::period}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::mult}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::div}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::and_op}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::equal}, common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::not_equal},
                              common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::less_than},
                              common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::greater_than},
                              common);
         table.set_production({sem::grammar_type::func_or_var_idnest, token_type::less_equal_than},
                              common);
         table.set_production(
            {sem::grammar_type::func_or_var_idnest, token_type::greater_equal_than}, common);
      }

      // <FuncOrVarIdnestTail>
      {
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::plus},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::minus},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::or_op},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::comma},
                              epsilon);
         table.set_production(
            {sem::grammar_type::func_or_var_idnest_tail, token_type::right_square}, epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::semi_colon},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::right_paren},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::colon},
                              epsilon);
         table.set_production(
            {sem::grammar_type::func_or_var_idnest_tail, token_type::period},
            {token_type::period, token_type::id, sem::grammar_type::func_or_var_idnest});
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::mult},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::div},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::and_op},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::equal},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::not_equal},
                              epsilon);
         table.set_production({sem::grammar_type::func_or_var_idnest_tail, token_type::less_than},
                              epsilon);
         table.set_production(
            {sem::grammar_type::func_or_var_idnest_tail, token_type::greater_than}, epsilon);
         table.set_production(
            {sem::grammar_type::func_or_var_idnest_tail, token_type::less_equal_than}, epsilon);
         table.set_production(
            {sem::grammar_type::func_or_var_idnest_tail, token_type::greater_equal_than}, epsilon);
      }

      // <FuncStatTail>
      {
         table.set_production({sem::grammar_type::func_stat_tail, token_type::left_square},
                              {sem::grammar_type::indice_rep, token_type::period, token_type::id,
                               sem::grammar_type::func_stat_tail});
         table.set_production({sem::grammar_type::func_stat_tail, token_type::left_paren},
                              {token_type::left_paren, sem::grammar_type::a_params,
                               token_type::right_paren, sem::grammar_type::func_stat_tail_idnest});
         table.set_production({sem::grammar_type::func_stat_tail, token_type::period},
                              {sem::grammar_type::indice_rep, token_type::period, token_type::id,
                               sem::grammar_type::func_stat_tail});
      }

      // <FuncStatTailIdnest>
      {
         table.set_production({sem::grammar_type::func_stat_tail_idnest, token_type::semi_colon},
                              epsilon);
         table.set_production(
            {sem::grammar_type::func_stat_tail_idnest, token_type::period},
            {token_type::period, token_type::id, sem::grammar_type::func_stat_tail});
      }

      // <Function>
      {
         table.set_production({sem::grammar_type::function, token_type::id_func},
                              {sem::grammar_type::func_head, sem::action_type::func_head,
                               sem::grammar_type::func_body, sem::action_type::func_body});
      }

      // <IndiceRep>
      {
         table.set_production({sem::grammar_type::indice_rep, token_type::plus}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::minus}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::or_op}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::comma}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::left_square},
                              {token_type::left_square, sem::grammar_type::expr,
                               token_type::right_square, sem::grammar_type::indice_rep});
         table.set_production({sem::grammar_type::indice_rep, token_type::right_square}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::assign}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::semi_colon}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::right_paren}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::colon}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::period}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::mult}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::div}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::and_op}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::equal}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::not_equal}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::less_than}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::greater_than}, epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::less_equal_than},
                              epsilon);
         table.set_production({sem::grammar_type::indice_rep, token_type::greater_equal_than},
                              epsilon);
      }

      // <Inherits>
      {
         table.set_production({sem::grammar_type::inherit, token_type::left_brace}, epsilon);
         table.set_production({sem::grammar_type::inherit, token_type::id_inherits},
                              {token_type::id_inherits, token_type::id, sem::action_type::value,
                               sem::action_type::inherit_decl, sem::grammar_type::nested_id});
      }

      // <IntNum>
      {
         table.set_production({sem::grammar_type::int_num, token_type::integer_lit},
                              {token_type::integer_lit, sem::action_type::value});
         table.set_production({sem::grammar_type::int_num, token_type::right_square}, epsilon);
      }

      //////////////////// HERE

      // <MemberDecl>
      {
         table.set_production({sem::grammar_type::member_decl, token_type::id},
                              {sem::grammar_type::var_decl, sem::action_type::var_decl});
         table.set_production({sem::grammar_type::member_decl, token_type::id_string},
                              {sem::grammar_type::var_decl, sem::action_type::var_decl});
         table.set_production({sem::grammar_type::member_decl, token_type::id_float},
                              {sem::grammar_type::var_decl, sem::action_type::var_decl});
         table.set_production({sem::grammar_type::member_decl, token_type::id_integer},
                              {sem::grammar_type::var_decl, sem::action_type::var_decl});
         table.set_production({sem::grammar_type::member_decl, token_type::id_func},
                              {sem::grammar_type::func_decl, sem::action_type::function_decl});
      }

      // <MethodBodyVar>
      {
         table.set_production({sem::grammar_type::method_body_var, token_type::id}, epsilon);
         table.set_production({sem::grammar_type::method_body_var, token_type::id_continue},
                              epsilon);
         table.set_production({sem::grammar_type::method_body_var, token_type::id_break}, epsilon);
         table.set_production({sem::grammar_type::method_body_var, token_type::id_return}, epsilon);
         table.set_production({sem::grammar_type::method_body_var, token_type::id_write}, epsilon);
         table.set_production({sem::grammar_type::method_body_var, token_type::id_read}, epsilon);
         table.set_production({sem::grammar_type::method_body_var, token_type::id_while}, epsilon);
         table.set_production({sem::grammar_type::method_body_var, token_type::id_if}, epsilon);
         table.set_production({sem::grammar_type::method_body_var, token_type::id_var},
                              {token_type::id_var, token_type::left_brace,
                               sem::grammar_type::var_decl_rep, sem::action_type::var_decl_list,
                               token_type::right_brace});
         table.set_production({sem::grammar_type::method_body_var, token_type::right_brace},
                              epsilon);
      }

      // <MultOp>
      {
         table.set_production({sem::grammar_type::mult_op, token_type::and_op},
                              {token_type::and_op, sem::action_type::value});
         table.set_production({sem::grammar_type::mult_op, token_type::div},
                              {token_type::div, sem::action_type::value});
         table.set_production({sem::grammar_type::mult_op, token_type::mult},
                              {token_type::mult, sem::action_type::value});
      }

      // <NestedId>
      {
         table.set_production({sem::grammar_type::nested_id, token_type::left_brace}, epsilon);
         table.set_production({sem::grammar_type::nested_id, token_type::comma},
                              {token_type::comma, token_type::id, sem::action_type::value,
                               sem::action_type::inherit_decl, sem::grammar_type::nested_id});
      }

      // <Prog>
      {
         symbol_array common{sem::grammar_type::class_decl, sem::action_type::class_decl_list,
                             sem::grammar_type::func_def,   sem::action_type::func_def_list,
                             token_type::id_main,           sem::grammar_type::func_body,
                             sem::action_type::func_body};
         table.set_production({sem::grammar_type::prog, token_type::id_main}, common);
         table.set_production({sem::grammar_type::prog, token_type::id_func}, common);
         table.set_production({sem::grammar_type::prog, token_type::id_class}, common);
      }

      // <RelOp>
      {
         table.set_production({sem::grammar_type::relop, token_type::greater_equal_than},
                              {token_type::greater_equal_than, sem::action_type::value});
         table.set_production({sem::grammar_type::relop, token_type::less_equal_than},
                              {token_type::less_equal_than, sem::action_type::value});
         table.set_production({sem::grammar_type::relop, token_type::greater_than},
                              {token_type::greater_than, sem::action_type::value});
         table.set_production({sem::grammar_type::relop, token_type::less_than},
                              {token_type::less_than, sem::action_type::value});
         table.set_production({sem::grammar_type::relop, token_type::not_equal},
                              {token_type::not_equal, sem::action_type::value});
         table.set_production({sem::grammar_type::relop, token_type::equal},
                              {token_type::equal, sem::action_type::value});
      }

      // <Sign>
      {
         table.set_production({sem::grammar_type::sign, token_type::plus}, {token_type::plus});
         table.set_production({sem::grammar_type::sign, token_type::minus}, {token_type::minus});
      }

      // <Start>
      {
         symbol_array common{sem::grammar_type::prog, sem::action_type::program};
         table.set_production({sem::grammar_type::start, token_type::id_main}, common);
         table.set_production({sem::grammar_type::start, token_type::id_func}, common);
         table.set_production({sem::grammar_type::start, token_type::id_class}, common);
      }

      // <StatBlock>
      {
         symbol_array common{sem::grammar_type::statement, sem::action_type::statement};
         table.set_production({sem::grammar_type::stat_block, token_type::id}, common);
         table.set_production({sem::grammar_type::stat_block, token_type::semi_colon}, epsilon);
         table.set_production({sem::grammar_type::stat_block, token_type::id_continue}, common);
         table.set_production({sem::grammar_type::stat_block, token_type::id_break}, common);
         table.set_production({sem::grammar_type::stat_block, token_type::id_return}, common);
         table.set_production({sem::grammar_type::stat_block, token_type::id_write}, common);
         table.set_production({sem::grammar_type::stat_block, token_type::id_read}, common);
         table.set_production({sem::grammar_type::stat_block, token_type::id_while}, common);
         table.set_production({sem::grammar_type::stat_block, token_type::id_else}, epsilon);
         table.set_production({sem::grammar_type::stat_block, token_type::id_if}, common);
         table.set_production({sem::grammar_type::stat_block, token_type::left_brace},
                              {token_type::left_brace, sem::grammar_type::statement_list,
                               sem::action_type::statement_list, token_type::right_brace});
      }

      ///////////////////// HERE

      // <Statement>
      {
         const auto key = sem::grammar_type::statement;
         table.set_production({key, token_type::id},
                              {sem::grammar_type::func_or_assign_stat, token_type::semi_colon});
         table.set_production({key, token_type::id_if},
                              {token_type::id_if, token_type::left_paren, sem::grammar_type::expr,
                               token_type::right_paren, token_type::id_then, token_type::id_else,
                               sem::action_type::if_statement, token_type::semi_colon});
         table.set_production({key, token_type::id_while},
                              {token_type::id_while, token_type::left_paren,
                               sem::grammar_type::expr, sem::action_type::expr,
                               token_type::right_paren, sem::grammar_type::stat_block,
                               sem::action_type::statement_block, sem::action_type::while_statement,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_read},
                              {token_type::id_read, token_type::left_paren,
                               sem::grammar_type::variable, token_type::right_paren,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_write},
                              {token_type::id_write, token_type::left_paren,
                               sem::grammar_type::expr, sem::action_type::expr,
                               sem::action_type::write_statement, token_type::right_paren,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_return},
                              {token_type::id_return, token_type::left_paren,
                               sem::grammar_type::expr, sem::action_type::expr,
                               sem::action_type::return_statement, token_type::right_paren,
                               token_type::semi_colon});
         table.set_production(
            {key, token_type::id_break},
            {token_type::id_break, sem::action_type::statement, token_type::semi_colon});
         table.set_production(
            {key, token_type::id_continue},
            {token_type::id_continue, sem::action_type::statement, token_type::semi_colon});
      }

      // <StatementList>
      {
         symbol_array common{sem::grammar_type::statement, sem::grammar_type::statement_list};
         const auto key = sem::grammar_type::statement_list;
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::right_brace}, epsilon);
         table.set_production({key, token_type::id_if}, common);
         table.set_production({key, token_type::id_while}, common);
         table.set_production({key, token_type::id_read}, common);
         table.set_production({key, token_type::id_write}, common);
         table.set_production({key, token_type::id_return}, common);
         table.set_production({key, token_type::id_break}, common);
         table.set_production({key, token_type::id_continue}, common);
      }

      // <Term>
      {
         symbol_array common{sem::grammar_type::factor, sem::action_type::factor_expr,
                             sem::grammar_type::term_tail, sem::action_type::term_tail_expr_list};
         table.set_production({sem::grammar_type::term, token_type::plus}, common);
         table.set_production({sem::grammar_type::term, token_type::minus}, common);
         table.set_production({sem::grammar_type::term, token_type::id}, common);
         table.set_production({sem::grammar_type::term, token_type::integer_lit}, common);
         table.set_production({sem::grammar_type::term, token_type::float_lit}, common);
         table.set_production({sem::grammar_type::term, token_type::str_lit}, common);
         table.set_production({sem::grammar_type::term, token_type::left_paren}, common);
         table.set_production({sem::grammar_type::term, token_type::not_op}, common);
         table.set_production({sem::grammar_type::term, token_type::qmark}, common);
      }

      // <TermTail>
      {
         symbol_array common{sem::grammar_type::mult_op,       sem::action_type::mult_op,
                             sem::grammar_type::factor,        sem::action_type::factor_expr,
                             sem::action_type::term_tail_expr, sem::grammar_type::term_tail};
         table.set_production({sem::grammar_type::term_tail, token_type::plus}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::minus}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::or_op}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::comma}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::right_square}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::semi_colon}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::right_paren}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::colon}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::mult}, common);
         table.set_production({sem::grammar_type::term_tail, token_type::div}, common);
         table.set_production({sem::grammar_type::term_tail, token_type::and_op}, common);
         table.set_production({sem::grammar_type::term_tail, token_type::equal}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::not_equal}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::less_than}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::greater_than}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::less_equal_than}, epsilon);
         table.set_production({sem::grammar_type::term_tail, token_type::greater_equal_than},
                              epsilon);
      }

      // <Type>
      {
         table.set_production({sem::grammar_type::type, token_type::id}, {token_type::id});
         table.set_production({sem::grammar_type::type, token_type::id_integer},
                              {token_type::id_integer});
         table.set_production({sem::grammar_type::type, token_type::id_float},
                              {token_type::id_float});
         table.set_production({sem::grammar_type::type, token_type::id_string},
                              {token_type::id_string});
      }

      // <VarDecl>
      {
         symbol_array common{sem::grammar_type::type,
                             sem::action_type::type,
                             token_type::id,
                             sem::action_type::value,
                             sem::grammar_type::array_size_rept,
                             sem::action_type::array_size_stmt_list,
                             token_type::semi_colon};
         table.set_production({sem::grammar_type::var_decl, token_type::id}, common);
         table.set_production({sem::grammar_type::var_decl, token_type::id_integer}, common);
         table.set_production({sem::grammar_type::var_decl, token_type::id_float}, common);
         table.set_production({sem::grammar_type::var_decl, token_type::id_string}, common);
      }

      // <VarDeclRep>
      {
         symbol_array common{sem::grammar_type::var_decl, sem::action_type::var_decl,
                             sem::grammar_type::var_decl_rep};
         const auto key = sem::grammar_type::var_decl_rep;
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::right_brace}, epsilon);
         table.set_production({key, token_type::id_integer}, common);
         table.set_production({key, token_type::id_float}, common);
         table.set_production({key, token_type::id_string}, common);
      }

      // <Variable>
      {
         table.set_production({sem::grammar_type::variable, token_type::id},
                              {token_type::id, sem::grammar_type::variable_idnest});
      }

      // <VariableIdnest>
      {
         symbol_array common{sem::grammar_type::indice_rep,
                             sem::grammar_type::variable_idnest_tail};
         table.set_production({sem::grammar_type::variable_idnest, token_type::left_square},
                              common);
         table.set_production({sem::grammar_type::variable_idnest, token_type::right_paren},
                              common);
         table.set_production({sem::grammar_type::variable_idnest, token_type::period}, common);
      }

      // <VariableIdnestTail>
      {
         const auto key = sem::grammar_type::variable_idnest_tail;
         table.set_production({key, token_type::right_paren}, epsilon);
         table.set_production(
            {key, token_type::period},
            {token_type::period, token_type::id, sem::grammar_type::variable_idnest});
      }

      // <Visibility>
      {
         table.set_production({sem::grammar_type::visibility, token_type::id}, epsilon);
         table.set_production({sem::grammar_type::visibility, token_type::id_func}, epsilon);
         table.set_production({sem::grammar_type::visibility, token_type::id_integer}, epsilon);
         table.set_production({sem::grammar_type::visibility, token_type::id_float}, epsilon);
         table.set_production({sem::grammar_type::visibility, token_type::id_string}, epsilon);
         table.set_production({sem::grammar_type::visibility, token_type::id_public},
                              {token_type::id_public, sem::action_type::value});
         table.set_production({sem::grammar_type::visibility, token_type::id_private},
                              {token_type::id_private, sem::action_type::value});
      }

      return table;
   }

   auto is_comment(const fr::lex_item& item) -> bool
   {
      using namespace grammar;
      return (item.type == token_type::block_cmt) || (item.type == token_type::line_cmt);
   }
} // namespace fr
