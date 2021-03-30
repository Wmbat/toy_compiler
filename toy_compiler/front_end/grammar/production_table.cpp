/**
 * @file symbol_table.cpp
 * @brief Implement the functions defined at symbol_table.hpp
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

#include <toy_compiler/front_end/grammar/production_table.hpp>

#include <utility>

namespace fr::grammar
{
   auto production_table::lookup(const key& k) const -> const production&
   {
      const auto col = static_cast<std::size_t>(k.second);
      const auto row = static_cast<std::size_t>(k.first);

      return m_data[col][row]; // NOLINT
   }
   auto production_table::lookup(const key& k) -> production&
   {
      const auto col = static_cast<std::size_t>(k.second);
      const auto row = static_cast<std::size_t>(k.first);

      return m_data[col][row]; // NOLINT
   }

   void production_table::set_production(const key& key, symbol_array tail, nullable_t nullable)
   {
      const auto col = static_cast<std::size_t>(key.second);
      const auto row = static_cast<std::size_t>(key.first);

      m_data.at(col).at(row) = production{key.first, std::move(tail), nullable}; // NOLINT
   }

   auto construct_production_table() -> const grammar::production_table
   {
      using namespace grammar;
      using namespace front::sem;

      production_table table{};
      symbol_array epsilon = {token_type::epsilon};

      // <AddOp>
      {
         const auto key = grammar_type::add_op;
         table.set_production({key, token_type::plus}, {token_type::plus});
         table.set_production({key, token_type::minus}, {token_type::minus});
         table.set_production({key, token_type::or_op}, {token_type::or_op});
      }

      // <aParams>
      {
         symbol_array common{grammar_type::expr, grammar_type::a_params_tail};

         const auto key = grammar_type::a_params;
         table.set_production({key, token_type::plus}, common);
         table.set_production({key, token_type::minus}, common);
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::integer_lit}, common);
         table.set_production({key, token_type::float_lit}, common);
         table.set_production({key, token_type::str_lit}, common);
         table.set_production({key, token_type::left_paren}, common);
         table.set_production({key, token_type::right_paren},
                              {token_type::epsilon, action::epsilon});
         table.set_production({key, token_type::not_op}, common);
         table.set_production({key, token_type::qmark}, common);
      }

      // <aParamsTail>
      {
         const auto key = grammar_type::a_params_tail;
         table.set_production({key, token_type::comma},
                              {token_type::comma, grammar_type::expr, grammar_type::a_params_tail});
         table.set_production({key, token_type::right_paren}, {epsilon[0], action::epsilon});
      }

      // <ArithExpr>
      {
         symbol_array common{grammar_type::term, grammar_type::arith_expr_tail};
         const auto key = grammar_type::arith_expr;
         table.set_production({key, token_type::minus}, common);
         table.set_production({key, token_type::plus}, common);
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::integer_lit}, common);
         table.set_production({key, token_type::float_lit}, common);
         table.set_production({key, token_type::str_lit}, common);
         table.set_production({key, token_type::left_paren}, common);
         table.set_production({key, token_type::not_op}, common);
         table.set_production({key, token_type::qmark}, common);
      }

      // <ArithExprTail>
      {
         symbol_array common{grammar_type::add_op, action::id_decl, grammar_type::term,
                             action::add_op, grammar_type::arith_expr_tail};
         const auto key = grammar_type::arith_expr_tail;
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
         const auto key = grammar_type::array_size_rept;
         table.set_production({key, token_type::comma}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::left_square},
                              {token_type::left_square, action::location_decl,
                               grammar_type::int_num, token_type::right_square,
                               action::location_decl, action::array_decl,
                               grammar_type::array_size_rept});
         table.set_production({key, token_type::semi_colon}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::right_paren}, {epsilon[0], action::epsilon});
      }

      // <AssignOp>
      {
         table.set_production({grammar_type::assign_op, token_type::assign}, {token_type::assign});
      }

      // <AssignStatTail>
      {
         table.set_production({grammar_type::assign_stat_tail, token_type::assign},
                              {grammar_type::assign_op, grammar_type::expr});
      }

      // <ClassDecl>
      {
         table.set_production(
            {grammar_type::class_decl, token_type::id_class},
            {token_type::id_class, action::location_decl, token_type::id, action::id_decl,
             grammar_type::inherit, action::compound_inheritance_decl, token_type::left_brace,
             grammar_type::class_decl_body, action::compound_member_decl, token_type::right_brace,
             token_type::semi_colon, action::class_decl, grammar_type::class_decl});
         table.set_production({grammar_type::class_decl, token_type::id_func},
                              {token_type::epsilon, action::epsilon});
         table.set_production({grammar_type::class_decl, token_type::id_main},
                              {token_type::epsilon, action::epsilon});
      }

      // <ClassDeclBody>
      {
         symbol_array common{grammar_type::visibility, grammar_type::member_decl,
                             action::member_decl, grammar_type::class_decl_body};

         const auto key = grammar_type::class_decl_body;
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::right_brace}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_func}, common);
         table.set_production({key, token_type::id_integer}, common);
         table.set_production({key, token_type::id_float}, common);
         table.set_production({key, token_type::id_string}, common);
         table.set_production({key, token_type::id_public}, common);
         table.set_production({key, token_type::id_private}, common);
      }

      // <ClassMethod>
      {
         table.set_production({grammar_type::class_method, token_type::double_colon},
                              {token_type::double_colon, token_type::id, action::id_decl});
         table.set_production({grammar_type::class_method, token_type::left_paren},
                              {epsilon[0], action::epsilon});
      }

      // <Expr>
      {
         symbol_array common{grammar_type::arith_expr, grammar_type::expr_tail};
         const auto key = grammar_type::expr;
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
         symbol_array common{grammar_type::relop, action::id_decl, grammar_type::arith_expr,
                             action::rel_op};
         const auto key = grammar_type::expr_tail;
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
         const auto key = grammar_type::factor;
         table.set_production(
            {key, token_type::minus},
            {grammar_type::sign, action::id_decl, grammar_type::factor, action::sign_expr});
         table.set_production(
            {key, token_type::plus},
            {grammar_type::sign, action::id_decl, grammar_type::factor, action::sign_expr});
         table.set_production({key, token_type::id},
                              {grammar_type::func_or_var, action::func_or_var_expr});
         table.set_production({key, token_type::float_lit},
                              {token_type::float_lit, action::float_expr});
         table.set_production({key, token_type::integer_lit},
                              {token_type::integer_lit, action::int_expr});
         table.set_production({key, token_type::str_lit}, {token_type::str_lit, action::str_expr});
         table.set_production({key, token_type::left_paren},
                              {token_type::left_paren, action::location_decl, grammar_type::expr,
                               action::priority_expr, token_type::right_paren});
         table.set_production({key, token_type::not_op},
                              {token_type::not_op, grammar_type::factor});
         table.set_production({key, token_type::qmark},
                              {token_type::qmark, action::location_decl, token_type::left_square,
                               grammar_type::expr, token_type::colon, grammar_type::expr,
                               token_type::colon, grammar_type::expr, token_type::right_square,
                               action::ternary_expr});
      }

      // <fParams>
      {
         symbol_array common{grammar_type::type,
                             action::type_decl,
                             token_type::id,
                             action::id_decl,
                             grammar_type::array_size_rept,
                             action::compound_array_decl,
                             action::variable_decl,
                             grammar_type::f_params_tail};
         table.set_production({grammar_type::f_params, token_type::id}, common);
         table.set_production({grammar_type::f_params, token_type::right_paren},
                              {epsilon[0], action::epsilon});
         table.set_production({grammar_type::f_params, token_type::id_integer}, common);
         table.set_production({grammar_type::f_params, token_type::id_float}, common);
         table.set_production({grammar_type::f_params, token_type::id_string}, common);
      }

      // <fParamsTail>
      {
         table.set_production({grammar_type::f_params_tail, token_type::comma},
                              {token_type::comma, grammar_type::type, action::type_decl,
                               token_type::id, action::id_decl, grammar_type::array_size_rept,
                               action::compound_array_decl, action::variable_decl,
                               grammar_type::f_params_tail});
         table.set_production({grammar_type::f_params_tail, token_type::right_paren},
                              {epsilon[0], action::epsilon});
      }

      // <FuncBody>
      {
         table.set_production({grammar_type::func_body, token_type::left_brace},
                              {token_type::left_brace, grammar_type::block_variable_decl,
                               action::compound_variable_decl, grammar_type::statement_list,
                               action::compound_stmt, token_type::right_brace});
      }

      // <FuncDecl>
      {
         table.set_production({grammar_type::func_decl, token_type::id_func},
                              {token_type::id_func, action::location_decl, token_type::id,
                               action::id_decl, token_type::left_paren, grammar_type::f_params,
                               action::compound_param_decl, token_type::right_paren,
                               token_type::colon, grammar_type::func_decl_tail, action::type_decl,
                               token_type::semi_colon});
      }

      // <FuncDeclTail>
      {
         table.set_production({grammar_type::func_decl_tail, token_type::id}, {grammar_type::type});
         table.set_production({grammar_type::func_decl_tail, token_type::id_void},
                              {token_type::id_void});
         table.set_production({grammar_type::func_decl_tail, token_type::id_integer},
                              {grammar_type::type});
         table.set_production({grammar_type::func_decl_tail, token_type::id_float},
                              {grammar_type::type});
         table.set_production({grammar_type::func_decl_tail, token_type::id_string},
                              {grammar_type::type});
      }

      // <FuncDef>
      {
         table.set_production(
            {grammar_type::func_def, token_type::id_func},
            {grammar_type::function, action::function_decl, grammar_type::func_def});
         table.set_production({grammar_type::func_def, token_type::id_main},
                              {epsilon[0], action::epsilon});
      }

      // <FuncHead>
      {
         table.set_production({grammar_type::func_head, token_type::id_func},
                              {token_type::id_func, action::location_decl, token_type::id,
                               action::id_decl, grammar_type::class_method, token_type::left_paren,
                               grammar_type::f_params, action::compound_param_decl,
                               token_type::right_paren, token_type::colon,
                               grammar_type::func_decl_tail, action::type_decl});
      }

      // <FuncOrAssignStat>
      {
         table.set_production({grammar_type::func_or_assign_stat, token_type::id},
                              {token_type::id, grammar_type::func_or_assign_stat_idnest});
      }

      // <FuncOrAssignStatIdnest>
      {
         symbol_array common{grammar_type::indice_rep,
                             grammar_type::func_or_assign_stat_idnest_var_tail};

         table.set_production({grammar_type::func_or_assign_stat_idnest, token_type::left_square},
                              common);
         table.set_production({grammar_type::func_or_assign_stat_idnest, token_type::assign},
                              common);
         table.set_production({grammar_type::func_or_assign_stat_idnest, token_type::left_paren},
                              {token_type::left_paren, grammar_type::a_params,
                               token_type::right_paren,
                               grammar_type::func_or_assign_stat_idnest_func_tail});
         table.set_production({grammar_type::func_or_assign_stat_idnest, token_type::period},
                              common);
      }

      // <FuncOrAssignStatIdnestFuncTail>
      {
         table.set_production(
            {grammar_type::func_or_assign_stat_idnest_func_tail, token_type::semi_colon}, epsilon);
         table.set_production(
            {grammar_type::func_or_assign_stat_idnest_func_tail, token_type::period},
            {token_type::period, token_type::id, grammar_type::func_stat_tail});
      }

      // <FuncOrAssignStatIdnestVarTail>
      {
         table.set_production(
            {grammar_type::func_or_assign_stat_idnest_var_tail, token_type::assign},
            {grammar_type::assign_stat_tail});
         table.set_production(
            {grammar_type::func_or_assign_stat_idnest_var_tail, token_type::period},
            {token_type::period, token_type::id, grammar_type::func_or_assign_stat_idnest});
      }

      // <FuncOrVar>
      {
         table.set_production({grammar_type::func_or_var, token_type::id},
                              {token_type::id, action::id_decl, grammar_type::func_or_var_idnest});
      }

      // <FuncOrVarIdnest>
      {
         symbol_array common{grammar_type::indice_rep, action::compound_array_index_access_decl,
                             action::var_expr, grammar_type::func_or_var_idnest_tail};

         table.set_production({grammar_type::func_or_var_idnest, token_type::plus}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::minus}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::or_op}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::comma}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::left_square}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::right_square}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::semi_colon}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::left_paren},
                              {token_type::left_paren, grammar_type::a_params,
                               token_type::right_paren, action::compound_parameter_expr_decl,
                               action::func_expr, grammar_type::func_or_var_idnest_tail});
         table.set_production({grammar_type::func_or_var_idnest, token_type::right_paren}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::colon}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::period}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::mult}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::div}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::and_op}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::equal}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::not_equal}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::less_than}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::greater_than}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::less_equal_than},
                              common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::greater_equal_than},
                              common);
      }

      // <FuncOrVarIdnestTail>
      {
         const auto key = grammar_type::func_or_var_idnest_tail;
         table.set_production({key, token_type::plus}, epsilon);
         table.set_production({key, token_type::minus}, epsilon);
         table.set_production({key, token_type::or_op}, epsilon);
         table.set_production({key, token_type::comma}, epsilon);
         table.set_production({key, token_type::right_square}, epsilon);
         table.set_production({key, token_type::semi_colon}, epsilon);
         table.set_production({key, token_type::right_paren}, epsilon);
         table.set_production({key, token_type::colon}, epsilon);
         table.set_production({key, token_type::period},
                              {token_type::period, token_type::id, action::id_decl,
                               grammar_type::func_or_var_idnest});
         table.set_production({key, token_type::mult}, epsilon);
         table.set_production({key, token_type::div}, epsilon);
         table.set_production({key, token_type::and_op}, epsilon);
         table.set_production({key, token_type::equal}, epsilon);
         table.set_production({key, token_type::not_equal}, epsilon);
         table.set_production({key, token_type::less_than}, epsilon);
         table.set_production({key, token_type::greater_than}, epsilon);
         table.set_production({key, token_type::less_equal_than}, epsilon);
         table.set_production(
            {grammar_type::func_or_var_idnest_tail, token_type::greater_equal_than}, epsilon);
      }

      // <FuncStatTail>
      {
         table.set_production({grammar_type::func_stat_tail, token_type::left_square},
                              {grammar_type::indice_rep, token_type::period, token_type::id,
                               grammar_type::func_stat_tail});
         table.set_production({grammar_type::func_stat_tail, token_type::left_paren},
                              {token_type::left_paren, grammar_type::a_params,
                               token_type::right_paren, grammar_type::func_stat_tail_idnest});
         table.set_production({grammar_type::func_stat_tail, token_type::period},
                              {grammar_type::indice_rep, token_type::period, token_type::id,
                               grammar_type::func_stat_tail});
      }

      // <FuncStatTailIdnest>
      {
         table.set_production({grammar_type::func_stat_tail_idnest, token_type::semi_colon},
                              epsilon);
         table.set_production({grammar_type::func_stat_tail_idnest, token_type::period},
                              {token_type::period, token_type::id, grammar_type::func_stat_tail});
      }

      // <Function>
      {
         table.set_production({grammar_type::function, token_type::id_func},
                              {grammar_type::func_head, action::function_head_decl,
                               grammar_type::func_body, action::function_body_decl});
      }

      // <IndiceRep>
      {
         const auto key = grammar_type::indice_rep;
         table.set_production({key, token_type::plus}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::minus}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::or_op}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::comma}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::left_square},
                              {token_type::left_square, action::location_decl, grammar_type::expr,
                               token_type::right_square, action::location_decl,
                               action::array_index_access_decl, grammar_type::indice_rep});
         table.set_production({key, token_type::right_square}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::assign}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::semi_colon}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::right_paren}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::colon}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::period}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::mult}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::div}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::and_op}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::equal}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::not_equal}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::less_than}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::greater_than}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::less_equal_than}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::greater_equal_than}, {epsilon[0], action::epsilon});
      }

      // <Inherits>
      {
         table.set_production({grammar_type::inherit, token_type::left_brace},
                              {epsilon[0], action::epsilon});
         table.set_production({grammar_type::inherit, token_type::id_inherits},
                              {token_type::id_inherits, token_type::id, action::inheritance_decl,
                               grammar_type::nested_id});
      }

      // <IntNum>
      {
         table.set_production({grammar_type::int_num, token_type::integer_lit},
                              {token_type::integer_lit, action::integer_literal});
         table.set_production({grammar_type::int_num, token_type::right_square},
                              {epsilon[0], action::epsilon});
      }

      //////////////////// HERE

      // <MemberDecl>
      {
         table.set_production({grammar_type::member_decl, token_type::id},
                              {grammar_type::var_decl, action::variable_decl});
         table.set_production({grammar_type::member_decl, token_type::id_string},
                              {grammar_type::var_decl, action::variable_decl});
         table.set_production({grammar_type::member_decl, token_type::id_float},
                              {grammar_type::var_decl, action::variable_decl});
         table.set_production({grammar_type::member_decl, token_type::id_integer},
                              {grammar_type::var_decl, action::variable_decl});
         table.set_production({grammar_type::member_decl, token_type::id_func},
                              {grammar_type::func_decl, action::member_function_decl});
      }

      // <MethodBodyVar>
      {
         const auto key = grammar_type::block_variable_decl;
         table.set_production({key, token_type::id}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_continue}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_break}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_return}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_write}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_read}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_while}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_if}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_var},
                              {token_type::id_var, token_type::left_brace,
                               grammar_type::var_decl_rep, token_type::right_brace});
         table.set_production({key, token_type::right_brace}, {epsilon[0], action::epsilon});
      }

      // <MultOp>
      {
         table.set_production({grammar_type::mult_op, token_type::and_op}, {token_type::and_op});
         table.set_production({grammar_type::mult_op, token_type::div}, {token_type::div});
         table.set_production({grammar_type::mult_op, token_type::mult}, {token_type::mult});
      }

      // <NestedId>
      {
         table.set_production({grammar_type::nested_id, token_type::left_brace},
                              {epsilon[0], action::epsilon});
         table.set_production(
            {grammar_type::nested_id, token_type::comma},
            {token_type::comma, token_type::id, action::inheritance_decl, grammar_type::nested_id});
      }

      // <Prog>
      {
         symbol_array common{
            grammar_type::class_decl,       action::compound_class_decl, grammar_type::func_def,
            action::compound_function_decl, token_type::id_main,         action::id_decl,
            grammar_type::func_body,        action::function_body_decl,  action::main_decl};
         table.set_production({grammar_type::prog, token_type::id_main}, common);
         table.set_production({grammar_type::prog, token_type::id_func}, common);
         table.set_production({grammar_type::prog, token_type::id_class}, common);
      }

      // <RelOp>
      {
         table.set_production({grammar_type::relop, token_type::greater_equal_than},
                              {token_type::greater_equal_than});
         table.set_production({grammar_type::relop, token_type::less_equal_than},
                              {token_type::less_equal_than});
         table.set_production({grammar_type::relop, token_type::greater_than},
                              {token_type::greater_than});
         table.set_production({grammar_type::relop, token_type::less_than},
                              {token_type::less_than});
         table.set_production({grammar_type::relop, token_type::not_equal},
                              {token_type::not_equal});
         table.set_production({grammar_type::relop, token_type::equal}, {token_type::equal});
      }

      // <Sign>
      {
         table.set_production({grammar_type::sign, token_type::plus}, {token_type::plus});
         table.set_production({grammar_type::sign, token_type::minus}, {token_type::minus});
      }

      // <Start>
      {
         symbol_array common{grammar_type::prog, action::translation_unit};
         table.set_production({grammar_type::start, token_type::id_main}, common);
         table.set_production({grammar_type::start, token_type::id_func}, common);
         table.set_production({grammar_type::start, token_type::id_class}, common);
      }

      // <StatBlock>
      {
         symbol_array common{grammar_type::statement};
         table.set_production({grammar_type::stat_block, token_type::id}, common);
         table.set_production({grammar_type::stat_block, token_type::semi_colon}, epsilon);
         table.set_production({grammar_type::stat_block, token_type::id_continue}, common);
         table.set_production({grammar_type::stat_block, token_type::id_break}, common);
         table.set_production({grammar_type::stat_block, token_type::id_return}, common);
         table.set_production({grammar_type::stat_block, token_type::id_write}, common);
         table.set_production({grammar_type::stat_block, token_type::id_read}, common);
         table.set_production({grammar_type::stat_block, token_type::id_while}, common);
         table.set_production({grammar_type::stat_block, token_type::id_else}, epsilon);
         table.set_production({grammar_type::stat_block, token_type::id_if}, common);
         table.set_production(
            {grammar_type::stat_block, token_type::left_brace},
            {token_type::left_brace, grammar_type::statement_list, token_type::right_brace});
      }

      ///////////////////// HERE

      // <Statement>
      {
         const auto key = grammar_type::statement;
         table.set_production({key, token_type::id},
                              {grammar_type::func_or_assign_stat, token_type::semi_colon});
         table.set_production({key, token_type::id_if},
                              {token_type::id_if, token_type::left_paren, grammar_type::expr,
                               token_type::right_paren, token_type::id_then, token_type::id_else,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_while},
                              {token_type::id_while, token_type::left_paren, grammar_type::expr,
                               token_type::right_paren, grammar_type::stat_block,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_read},
                              {token_type::id_read, token_type::left_paren, grammar_type::variable,
                               token_type::right_paren, token_type::semi_colon});
         table.set_production({key, token_type::id_write},
                              {token_type::id_write, action::id_decl, token_type::left_paren,
                               grammar_type::expr, action::write_stmt, token_type::right_paren,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_return},
                              {token_type::id_return, action::id_decl, token_type::left_paren,
                               grammar_type::expr, action::return_stmt, token_type::right_paren,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_break},
                              {token_type::id_break, action::break_stmt, token_type::semi_colon});
         table.set_production(
            {key, token_type::id_continue},
            {token_type::id_continue, action::continue_stmt, token_type::semi_colon});
      }

      // <StatementList>
      {
         symbol_array common{grammar_type::statement, grammar_type::statement_list};
         const auto key = grammar_type::statement_list;
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::right_brace}, {epsilon[0], action::epsilon});
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
         symbol_array common{grammar_type::factor, grammar_type::term_tail};
         table.set_production({grammar_type::term, token_type::plus}, common);
         table.set_production({grammar_type::term, token_type::minus}, common);
         table.set_production({grammar_type::term, token_type::id}, common);
         table.set_production({grammar_type::term, token_type::integer_lit}, common);
         table.set_production({grammar_type::term, token_type::float_lit}, common);
         table.set_production({grammar_type::term, token_type::str_lit}, common);
         table.set_production({grammar_type::term, token_type::left_paren}, common);
         table.set_production({grammar_type::term, token_type::not_op}, common);
         table.set_production({grammar_type::term, token_type::qmark}, common);
      }

      // <TermTail>
      {
         symbol_array common{grammar_type::mult_op, action::id_decl, grammar_type::factor,
                             action::mult_op, grammar_type::term_tail};
         const auto key = grammar_type::term_tail;
         table.set_production({key, token_type::plus}, epsilon);
         table.set_production({key, token_type::minus}, epsilon);
         table.set_production({key, token_type::or_op}, epsilon);
         table.set_production({key, token_type::comma}, epsilon);
         table.set_production({key, token_type::right_square}, epsilon);
         table.set_production({key, token_type::semi_colon}, epsilon);
         table.set_production({key, token_type::right_paren}, epsilon);
         table.set_production({key, token_type::colon}, epsilon);
         table.set_production({key, token_type::mult}, common);
         table.set_production({key, token_type::div}, common);
         table.set_production({key, token_type::and_op}, common);
         table.set_production({key, token_type::equal}, epsilon);
         table.set_production({key, token_type::not_equal}, epsilon);
         table.set_production({key, token_type::less_than}, epsilon);
         table.set_production({key, token_type::greater_than}, epsilon);
         table.set_production({key, token_type::less_equal_than}, epsilon);
         table.set_production({key, token_type::greater_equal_than}, epsilon);
      }

      // <Type>
      {
         table.set_production({grammar_type::type, token_type::id}, {token_type::id});
         table.set_production({grammar_type::type, token_type::id_integer},
                              {token_type::id_integer});
         table.set_production({grammar_type::type, token_type::id_float}, {token_type::id_float});
         table.set_production({grammar_type::type, token_type::id_string}, {token_type::id_string});
      }

      // <VarDecl>
      {
         symbol_array common{grammar_type::type,
                             action::type_decl,
                             token_type::id,
                             action::id_decl,
                             grammar_type::array_size_rept,
                             action::compound_array_decl,
                             token_type::semi_colon};
         table.set_production({grammar_type::var_decl, token_type::id}, common);
         table.set_production({grammar_type::var_decl, token_type::id_integer}, common);
         table.set_production({grammar_type::var_decl, token_type::id_float}, common);
         table.set_production({grammar_type::var_decl, token_type::id_string}, common);
      }

      // <VarDeclRep>
      {
         symbol_array common{grammar_type::var_decl, action::variable_decl,
                             grammar_type::var_decl_rep};
         const auto key = grammar_type::var_decl_rep;
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::right_brace}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_integer}, common);
         table.set_production({key, token_type::id_float}, common);
         table.set_production({key, token_type::id_string}, common);
      }

      // <Variable>
      {
         table.set_production({grammar_type::variable, token_type::id},
                              {token_type::id, grammar_type::variable_idnest});
      }

      // <VariableIdnest>
      {
         symbol_array common{grammar_type::indice_rep, grammar_type::variable_idnest_tail};
         table.set_production({grammar_type::variable_idnest, token_type::left_square}, common);
         table.set_production({grammar_type::variable_idnest, token_type::right_paren}, common);
         table.set_production({grammar_type::variable_idnest, token_type::period}, common);
      }

      // <VariableIdnestTail>
      {
         const auto key = grammar_type::variable_idnest_tail;
         table.set_production({key, token_type::right_paren}, epsilon);
         table.set_production({key, token_type::period},
                              {token_type::period, token_type::id, grammar_type::variable_idnest});
      }

      // <Visibility>
      {
         const auto key = grammar_type::visibility;
         table.set_production({key, token_type::id}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_func}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_integer}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_float}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_string}, {epsilon[0], action::epsilon});
         table.set_production({key, token_type::id_public},
                              {token_type::id_public, action::visibily_decl});
         table.set_production({key, token_type::id_private},
                              {token_type::id_private, action::visibily_decl});
      }

      return table;
   }
} // namespace fr::grammar
