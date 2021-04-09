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
      symbol_array epsilon = {token_type::e_epsilon};

      // <AddOp>
      {
         const auto key = grammar_type::add_op;
         table.set_production({key, token_type::e_plus}, {token_type::e_plus});
         table.set_production({key, token_type::e_minus}, {token_type::e_minus});
         table.set_production({key, token_type::e_or}, {token_type::e_or});
      }

      // <aParams>
      {
         symbol_array common{grammar_type::expr, grammar_type::a_params_tail};

         const auto key = grammar_type::a_params;
         table.set_production({key, token_type::e_plus}, common);
         table.set_production({key, token_type::e_minus}, common);
         table.set_production({key, token_type::e_id}, common);
         table.set_production({key, token_type::e_integer_lit}, common);
         table.set_production({key, token_type::e_float_lit}, common);
         table.set_production({key, token_type::e_str_lit}, common);
         table.set_production({key, token_type::e_left_paren}, common);
         table.set_production({key, token_type::e_right_paren},
                              {token_type::e_epsilon, action::e_epsilon});
         table.set_production({key, token_type::e_not}, common);
         table.set_production({key, token_type::e_qmark}, common);
      }

      // <aParamsTail>
      {
         const auto key = grammar_type::a_params_tail;
         table.set_production(
            {key, token_type::e_comma},
            {token_type::e_comma, grammar_type::expr, grammar_type::a_params_tail});
         table.set_production({key, token_type::e_right_paren}, {epsilon[0], action::e_epsilon});
      }

      // <ArithExpr>
      {
         symbol_array common{grammar_type::term, grammar_type::arith_expr_tail};
         const auto key = grammar_type::arith_expr;
         table.set_production({key, token_type::e_minus}, common);
         table.set_production({key, token_type::e_plus}, common);
         table.set_production({key, token_type::e_id}, common);
         table.set_production({key, token_type::e_integer_lit}, common);
         table.set_production({key, token_type::e_float_lit}, common);
         table.set_production({key, token_type::e_str_lit}, common);
         table.set_production({key, token_type::e_left_paren}, common);
         table.set_production({key, token_type::e_not}, common);
         table.set_production({key, token_type::e_qmark}, common);
      }

      // <ArithExprTail>
      {
         symbol_array common{grammar_type::add_op, action::e_id_decl, grammar_type::term,
                             action::e_add_op, grammar_type::arith_expr_tail};
         const auto key = grammar_type::arith_expr_tail;
         table.set_production({key, token_type::e_plus}, common);
         table.set_production({key, token_type::e_minus}, common);
         table.set_production({key, token_type::e_or}, common);
         table.set_production({key, token_type::e_comma}, epsilon);
         table.set_production({key, token_type::e_right_square}, epsilon);
         table.set_production({key, token_type::e_semi_colon}, epsilon);
         table.set_production({key, token_type::e_right_paren}, epsilon);
         table.set_production({key, token_type::e_colon}, epsilon);
         table.set_production({key, token_type::e_equal}, epsilon);
         table.set_production({key, token_type::e_not_equal}, epsilon);
         table.set_production({key, token_type::e_less_than}, epsilon);
         table.set_production({key, token_type::e_greater_thane}, epsilon);
         table.set_production({key, token_type::e_less_equal_than}, epsilon);
         table.set_production({key, token_type::e_greater_equal_than}, epsilon);
      }

      // <ArraySizeRept>
      {
         const auto key = grammar_type::array_size_rept;
         table.set_production({key, token_type::e_comma}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_left_square},
                              {token_type::e_left_square, action::e_location_decl,
                               grammar_type::int_num, token_type::e_right_square,
                               action::e_location_decl, action::e_array_decl,
                               grammar_type::array_size_rept});
         table.set_production({key, token_type::e_semi_colon}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_right_paren}, {epsilon[0], action::e_epsilon});
      }

      // <AssignOp>
      {
         table.set_production({grammar_type::assign_op, token_type::e_assign},
                              {token_type::e_assign});
      }

      // <AssignStatTail>
      {
         table.set_production(
            {grammar_type::assign_stat_tail, token_type::e_assign},
            {grammar_type::assign_op, action::e_id_decl, grammar_type::expr, action::e_assign_op});
      }

      // <ClassDecl>
      {
         table.set_production(
            {grammar_type::class_decl, token_type::e_class},
            {token_type::e_class, action::e_location_decl, token_type::e_id, action::e_id_decl,
             grammar_type::inherit, action::e_compound_inheritance_decl, token_type::e_left_brace,
             grammar_type::class_decl_body, action::e_compound_member_decl, token_type::e_right_brace,
             token_type::e_semi_colon, action::e_class_decl, grammar_type::class_decl});
         table.set_production({grammar_type::class_decl, token_type::e_func},
                              {token_type::e_epsilon, action::e_epsilon});
         table.set_production({grammar_type::class_decl, token_type::e_main},
                              {token_type::e_epsilon, action::e_epsilon});
      }

      // <ClassDeclBody>
      {
         symbol_array common{grammar_type::visibility, grammar_type::member_decl,
                             grammar_type::class_decl_body};

         const auto key = grammar_type::class_decl_body;
         table.set_production({key, token_type::e_id}, common);
         table.set_production({key, token_type::e_right_brace}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_func}, common);
         table.set_production({key, token_type::e_integer}, common);
         table.set_production({key, token_type::e_float}, common);
         table.set_production({key, token_type::e_string}, common);
         table.set_production({key, token_type::e_public}, common);
         table.set_production({key, token_type::e_private}, common);
      }

      // <ClassMethod>
      {
         table.set_production({grammar_type::class_method, token_type::e_double_colon},
                              {token_type::e_double_colon, token_type::e_id, action::e_id_decl});
         table.set_production({grammar_type::class_method, token_type::e_left_paren},
                              {epsilon[0], action::e_epsilon});
      }

      // <Expr>
      {
         symbol_array common{grammar_type::arith_expr, grammar_type::expr_tail};
         const auto key = grammar_type::expr;
         table.set_production({key, token_type::e_plus}, common);
         table.set_production({key, token_type::e_minus}, common);
         table.set_production({key, token_type::e_id}, common);
         table.set_production({key, token_type::e_integer_lit}, common);
         table.set_production({key, token_type::e_float_lit}, common);
         table.set_production({key, token_type::e_str_lit}, common);
         table.set_production({key, token_type::e_left_paren}, common);
         table.set_production({key, token_type::e_not}, common);
         table.set_production({key, token_type::e_qmark}, common);
      }

      // <ExprTail>
      {
         symbol_array common{grammar_type::relop, action::e_id_decl, grammar_type::arith_expr,
                             action::e_rel_op};
         const auto key = grammar_type::expr_tail;
         table.set_production({key, token_type::e_comma}, epsilon);
         table.set_production({key, token_type::e_right_square}, epsilon);
         table.set_production({key, token_type::e_semi_colon}, epsilon);
         table.set_production({key, token_type::e_right_paren}, epsilon);
         table.set_production({key, token_type::e_colon}, epsilon);
         table.set_production({key, token_type::e_equal}, common);
         table.set_production({key, token_type::e_not_equal}, common);
         table.set_production({key, token_type::e_less_than}, common);
         table.set_production({key, token_type::e_greater_thane}, common);
         table.set_production({key, token_type::e_less_equal_than}, common);
         table.set_production({key, token_type::e_greater_equal_than}, common);
      }

      // <Factor>
      {
         const auto key = grammar_type::factor;
         table.set_production(
            {key, token_type::e_minus},
            {grammar_type::sign, action::e_id_decl, grammar_type::factor, action::e_sign_expr});
         table.set_production(
            {key, token_type::e_plus},
            {grammar_type::sign, action::e_id_decl, grammar_type::factor, action::e_sign_expr});
         table.set_production({key, token_type::e_id}, {grammar_type::func_or_var});
         table.set_production({key, token_type::e_float_lit},
                              {token_type::e_float_lit, action::e_float_expr});
         table.set_production({key, token_type::e_integer_lit},
                              {token_type::e_integer_lit, action::e_int_expr});
         table.set_production({key, token_type::e_str_lit},
                              {token_type::e_str_lit, action::e_str_expr});
         table.set_production({key, token_type::e_left_paren},
                              {token_type::e_left_paren, action::e_location_decl,
                               grammar_type::expr, action::e_priority_expr,
                               token_type::e_right_paren});
         table.set_production({key, token_type::e_not}, {token_type::e_not, grammar_type::factor});
         table.set_production({key, token_type::e_qmark},
                              {token_type::e_qmark, action::e_location_decl,
                               token_type::e_left_square, grammar_type::expr, token_type::e_colon,
                               grammar_type::expr, token_type::e_colon, grammar_type::expr,
                               token_type::e_right_square, action::e_ternary_expr});
      }

      // <fParams>
      {
         symbol_array common{
            grammar_type::type,    action::e_type_decl,           token_type::e_id,
            action::e_id_decl,     grammar_type::array_size_rept, action::e_compound_array_decl,
            action::e_variable_decl, grammar_type::f_params_tail};
         table.set_production({grammar_type::f_params, token_type::e_id}, common);
         table.set_production({grammar_type::f_params, token_type::e_right_paren},
                              {epsilon[0], action::e_epsilon});
         table.set_production({grammar_type::f_params, token_type::e_integer}, common);
         table.set_production({grammar_type::f_params, token_type::e_float}, common);
         table.set_production({grammar_type::f_params, token_type::e_string}, common);
      }

      // <fParamsTail>
      {
         table.set_production({grammar_type::f_params_tail, token_type::e_comma},
                              {token_type::e_comma, grammar_type::type, action::e_type_decl,
                               token_type::e_id, action::e_id_decl, grammar_type::array_size_rept,
                               action::e_compound_array_decl, action::e_variable_decl,
                               grammar_type::f_params_tail});
         table.set_production({grammar_type::f_params_tail, token_type::e_right_paren},
                              {epsilon[0], action::e_epsilon});
      }

      // <FuncBody>
      {
         table.set_production({grammar_type::func_body, token_type::e_left_brace},
                              {token_type::e_left_brace, grammar_type::block_variable_decl,
                               action::e_compound_variable_decl, grammar_type::statement_list,
                               action::e_compound_stmt, token_type::e_right_brace});
      }

      // <FuncDecl>
      {
         table.set_production({grammar_type::func_decl, token_type::e_func},
                              {token_type::e_func, action::e_location_decl, token_type::e_id,
                               action::e_id_decl, token_type::e_left_paren, grammar_type::f_params,
                               action::e_compound_param_decl, token_type::e_right_paren,
                               token_type::e_colon, grammar_type::func_decl_tail,
                               action::e_type_decl, token_type::e_semi_colon});
      }

      // <FuncDeclTail>
      {
         table.set_production({grammar_type::func_decl_tail, token_type::e_id},
                              {grammar_type::type});
         table.set_production({grammar_type::func_decl_tail, token_type::e_void},
                              {token_type::e_void});
         table.set_production({grammar_type::func_decl_tail, token_type::e_integer},
                              {grammar_type::type});
         table.set_production({grammar_type::func_decl_tail, token_type::e_float},
                              {grammar_type::type});
         table.set_production({grammar_type::func_decl_tail, token_type::e_string},
                              {grammar_type::type});
      }

      // <FuncDef>
      {
         table.set_production(
            {grammar_type::func_def, token_type::e_func},
            {grammar_type::function, action::e_func_decl, grammar_type::func_def});
         table.set_production({grammar_type::func_def, token_type::e_main},
                              {epsilon[0], action::e_epsilon});
      }

      // <FuncHead>
      {
         table.set_production(
            {grammar_type::func_head, token_type::e_func},
            {token_type::e_func, action::e_location_decl, token_type::e_id, action::e_id_decl,
             grammar_type::class_method, token_type::e_left_paren, grammar_type::f_params,
             action::e_compound_param_decl, token_type::e_right_paren, token_type::e_colon,
             grammar_type::func_decl_tail, action::e_type_decl});
      }

      // <FuncOrAssignStat>
      {
         table.set_production(
            {grammar_type::func_or_assign_stat, token_type::e_id},
            {token_type::e_id, action::e_id_decl, grammar_type::func_or_assign_stat_idnest});
      }

      // <FuncOrAssignStatIdnest>
      {
         symbol_array common{grammar_type::indice_rep, action::e_compound_array_index_access_decl,
                             action::e_var_expr, action::e_dot_op,
                             grammar_type::func_or_assign_stat_idnest_var_tail};

         const auto key = grammar_type::func_or_assign_stat_idnest;
         table.set_production({key, token_type::e_left_square}, common);
         table.set_production({key, token_type::e_assign}, common);
         table.set_production({key, token_type::e_left_paren},
                              {token_type::e_left_paren, grammar_type::a_params,
                               token_type::e_right_paren, action::e_compound_parameter_expr_decl,
                               action::e_func_expr, action::e_dot_op,
                               grammar_type::func_or_assign_stat_idnest_func_tail});
         table.set_production({key, token_type::e_dot}, common);
      }

      // <FuncOrAssignStatIdnestFuncTail>
      {
         const auto key = grammar_type::func_or_assign_stat_idnest_func_tail;
         table.set_production({key, token_type::e_semi_colon},
                              {epsilon[0], action::e_func_or_assign_stmt});
         table.set_production({key, token_type::e_dot},
                              {token_type::e_dot, action::e_dot_decl, token_type::e_id,
                               action::e_id_decl, grammar_type::func_stat_tail});
      }

      // <FuncOrAssignStatIdnestVarTail>
      {
         const auto key = grammar_type::func_or_assign_stat_idnest_var_tail;
         table.set_production({key, token_type::e_assign},
                              {grammar_type::assign_stat_tail, action::e_assign_stmt});
         table.set_production({key, token_type::e_dot},
                              {token_type::e_dot, action::e_dot_decl, token_type::e_id,
                               action::e_id_decl, grammar_type::func_or_assign_stat_idnest});
      }

      // <FuncOrVar>
      {
         table.set_production(
            {grammar_type::func_or_var, token_type::e_id},
            {token_type::e_id, action::e_id_decl, grammar_type::func_or_var_idnest});
      }

      // <FuncOrVarIdnest>
      {
         symbol_array common{grammar_type::indice_rep, action::e_compound_array_index_access_decl,
                             action::e_var_expr, action::e_dot_op,
                             grammar_type::func_or_var_idnest_tail};

         table.set_production({grammar_type::func_or_var_idnest, token_type::e_plus}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_minus}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_or}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_comma}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_left_square},
                              common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_right_square},
                              common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_semi_colon}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_left_paren},
                              {token_type::e_left_paren, grammar_type::a_params,
                               token_type::e_right_paren, action::e_compound_parameter_expr_decl,
                               action::e_func_expr, action::e_dot_op,
                               grammar_type::func_or_var_idnest_tail});
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_right_paren},
                              common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_colon}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_dot}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_mult}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_div}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_and}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_equal}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_not_equal}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_less_than}, common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_greater_thane},
                              common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_less_equal_than},
                              common);
         table.set_production({grammar_type::func_or_var_idnest, token_type::e_greater_equal_than},
                              common);
      }

      // <FuncOrVarIdnestTail>
      {
         const auto key = grammar_type::func_or_var_idnest_tail;
         table.set_production({key, token_type::e_plus}, epsilon);
         table.set_production({key, token_type::e_minus}, epsilon);
         table.set_production({key, token_type::e_or}, epsilon);
         table.set_production({key, token_type::e_comma}, epsilon);
         table.set_production({key, token_type::e_right_square}, epsilon);
         table.set_production({key, token_type::e_semi_colon}, epsilon);
         table.set_production({key, token_type::e_right_paren}, epsilon);
         table.set_production({key, token_type::e_colon}, epsilon);
         table.set_production({key, token_type::e_dot},
                              {token_type::e_dot, action::e_dot_decl, token_type::e_id,
                               action::e_id_decl, grammar_type::func_or_var_idnest});
         table.set_production({key, token_type::e_mult}, epsilon);
         table.set_production({key, token_type::e_div}, epsilon);
         table.set_production({key, token_type::e_and}, epsilon);
         table.set_production({key, token_type::e_equal}, epsilon);
         table.set_production({key, token_type::e_not_equal}, epsilon);
         table.set_production({key, token_type::e_less_than}, epsilon);
         table.set_production({key, token_type::e_greater_thane}, epsilon);
         table.set_production({key, token_type::e_less_equal_than}, epsilon);
         table.set_production(
            {grammar_type::func_or_var_idnest_tail, token_type::e_greater_equal_than}, epsilon);
      }

      // <FuncStatTail>
      {
         constexpr auto key = grammar_type::func_stat_tail;
         table.set_production({key, token_type::e_left_square},
                              {grammar_type::indice_rep, action::e_compound_array_index_access_decl,
                               action::e_var_expr, action::e_dot_op, token_type::e_dot,
                               action::e_dot_decl, token_type::e_id, action::e_id_decl,
                               grammar_type::func_stat_tail});
         table.set_production({key, token_type::e_left_paren},
                              {token_type::e_left_paren, grammar_type::a_params,
                               token_type::e_right_paren, action::e_compound_parameter_expr_decl,
                               action::e_func_expr, action::e_dot_op,
                               grammar_type::func_stat_tail_idnest});
         table.set_production({key, token_type::e_dot},
                              {grammar_type::indice_rep, action::e_compound_array_index_access_decl,
                               action::e_var_expr, action::e_dot_op, token_type::e_dot,
                               action::e_dot_decl, token_type::e_id, action::e_id_decl,
                               grammar_type::func_stat_tail});
      }

      // <FuncStatTailIdnest>
      {
         table.set_production({grammar_type::func_stat_tail_idnest, token_type::e_semi_colon},
                              epsilon);
         table.set_production({grammar_type::func_stat_tail_idnest, token_type::e_dot},
                              {token_type::e_dot, action::e_dot_decl, token_type::e_id,
                               action::e_id_decl, grammar_type::func_stat_tail});
      }

      // <Function>
      {
         table.set_production({grammar_type::function, token_type::e_func},
                              {grammar_type::func_head, action::e_func_head_decl,
                               grammar_type::func_body, action::e_func_body_decl});
      }

      // <IndiceRep>
      {
         const auto key = grammar_type::indice_rep;
         table.set_production({key, token_type::e_plus}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_minus}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_or}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_comma}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_left_square},
                              {token_type::e_left_square, action::e_location_decl,
                               grammar_type::expr, token_type::e_right_square,
                               action::e_location_decl, action::e_array_index_access_decl,
                               grammar_type::indice_rep});
         table.set_production({key, token_type::e_right_square}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_assign}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_semi_colon}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_right_paren}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_colon}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_dot}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_mult}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_div}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_and}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_equal}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_not_equal}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_less_than}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_greater_thane}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_less_equal_than},
                              {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_greater_equal_than},
                              {epsilon[0], action::e_epsilon});
      }

      // <Inherits>
      {
         table.set_production({grammar_type::inherit, token_type::e_left_brace},
                              {epsilon[0], action::e_epsilon});
         table.set_production({grammar_type::inherit, token_type::e_inherits},
                              {token_type::e_inherits, token_type::e_id, action::e_inheritance_decl,
                               grammar_type::nested_id});
      }

      // <IntNum>
      {
         table.set_production({grammar_type::int_num, token_type::e_integer_lit},
                              {token_type::e_integer_lit, action::e_integer_literal});
         table.set_production({grammar_type::int_num, token_type::e_right_square},
                              {epsilon[0], action::e_epsilon});
      }

      //////////////////// HERE

      // <MemberDecl>
      {
         constexpr auto key = grammar_type::member_decl;
         table.set_production({key, token_type::e_id},
                              {grammar_type::var_decl, action::e_member_var_decl});
         table.set_production({key, token_type::e_string},
                              {grammar_type::var_decl, action::e_member_var_decl});
         table.set_production({key, token_type::e_float},
                              {grammar_type::var_decl, action::e_member_var_decl});
         table.set_production({key, token_type::e_integer},
                              {grammar_type::var_decl, action::e_member_var_decl});
         table.set_production({key, token_type::e_func},
                              {grammar_type::func_decl, action::e_member_func_decl});
      }

      // <MethodBodyVar>
      {
         const auto key = grammar_type::block_variable_decl;
         table.set_production({key, token_type::e_id}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_continue}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_break}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_return}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_write}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_read}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_while}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_if}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_var},
                              {token_type::e_var, token_type::e_left_brace,
                               grammar_type::var_decl_rep, token_type::e_right_brace});
         table.set_production({key, token_type::e_right_brace}, {epsilon[0], action::e_epsilon});
      }

      // <MultOp>
      {
         table.set_production({grammar_type::mult_op, token_type::e_and}, {token_type::e_and});
         table.set_production({grammar_type::mult_op, token_type::e_div}, {token_type::e_div});
         table.set_production({grammar_type::mult_op, token_type::e_mult}, {token_type::e_mult});
      }

      // <NestedId>
      {
         table.set_production({grammar_type::nested_id, token_type::e_left_brace},
                              {epsilon[0], action::e_epsilon});
         table.set_production({grammar_type::nested_id, token_type::e_comma},
                              {token_type::e_comma, token_type::e_id, action::e_inheritance_decl,
                               grammar_type::nested_id});
      }

      // <Prog>
      {
         symbol_array common{
            grammar_type::class_decl,     action::e_compound_class_decl, grammar_type::func_def,
            action::e_compound_func_decl, token_type::e_main,          action::e_id_decl,
            grammar_type::func_body,      action::e_func_body_decl,    action::e_main_decl};
         table.set_production({grammar_type::prog, token_type::e_main}, common);
         table.set_production({grammar_type::prog, token_type::e_func}, common);
         table.set_production({grammar_type::prog, token_type::e_class}, common);
      }

      // <RelOp>
      {
         table.set_production({grammar_type::relop, token_type::e_greater_equal_than},
                              {token_type::e_greater_equal_than});
         table.set_production({grammar_type::relop, token_type::e_less_equal_than},
                              {token_type::e_less_equal_than});
         table.set_production({grammar_type::relop, token_type::e_greater_thane},
                              {token_type::e_greater_thane});
         table.set_production({grammar_type::relop, token_type::e_less_than},
                              {token_type::e_less_than});
         table.set_production({grammar_type::relop, token_type::e_not_equal},
                              {token_type::e_not_equal});
         table.set_production({grammar_type::relop, token_type::e_equal}, {token_type::e_equal});
      }

      // <Sign>
      {
         table.set_production({grammar_type::sign, token_type::e_plus}, {token_type::e_plus});
         table.set_production({grammar_type::sign, token_type::e_minus}, {token_type::e_minus});
      }

      // <Start>
      {
         symbol_array common{grammar_type::prog, action::e_translation_unit};
         table.set_production({grammar_type::start, token_type::e_main}, common);
         table.set_production({grammar_type::start, token_type::e_func}, common);
         table.set_production({grammar_type::start, token_type::e_class}, common);
      }

      // <StatBlock>
      {
         symbol_array common{grammar_type::statement};

         const auto key = grammar_type::stat_block;
         table.set_production({key, token_type::e_id}, common);
         table.set_production({key, token_type::e_semi_colon}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_continue}, common);
         table.set_production({key, token_type::e_break}, common);
         table.set_production({key, token_type::e_return}, common);
         table.set_production({key, token_type::e_write}, common);
         table.set_production({key, token_type::e_read}, common);
         table.set_production({key, token_type::e_while}, common);
         table.set_production({key, token_type::e_else}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_if}, common);
         table.set_production({key, token_type::e_left_brace},
                              {token_type::e_left_brace, grammar_type::statement_list,
                               token_type::e_right_brace, action::e_compound_stmt});
      }

      // <Statement>
      {
         const auto key = grammar_type::statement;
         table.set_production({key, token_type::e_id},
                              {grammar_type::func_or_assign_stat, token_type::e_semi_colon});
         table.set_production({key, token_type::e_if},
                              {token_type::e_if, action::e_location_decl, token_type::e_left_paren,
                               grammar_type::expr, token_type::e_right_paren, token_type::e_then,
                               grammar_type::stat_block, action::e_stmt_block_decl,
                               token_type::e_else, grammar_type::stat_block,
                               action::e_stmt_block_decl, token_type::e_semi_colon,
                               action::e_if_stmt});
         table.set_production(
            {key, token_type::e_while},
            {token_type::e_while, action::e_location_decl, token_type::e_left_paren,
             grammar_type::expr, token_type::e_right_paren, grammar_type::stat_block,
             action::e_stmt_block_decl, action::e_while_stmt, token_type::e_semi_colon});
         table.set_production({key, token_type::e_read},
                              {token_type::e_read, action::e_location_decl,
                               token_type::e_left_paren, grammar_type::variable,
                               action::e_compound_var_expr, action::e_read_stmt,
                               token_type::e_right_paren, token_type::e_semi_colon});
         table.set_production({key, token_type::e_write},
                              {token_type::e_write, action::e_id_decl, token_type::e_left_paren,
                               grammar_type::expr, action::e_write_stmt, token_type::e_right_paren,
                               token_type::e_semi_colon});
         table.set_production({key, token_type::e_return},
                              {token_type::e_return, action::e_id_decl, token_type::e_left_paren,
                               grammar_type::expr, action::e_return_stmt, token_type::e_right_paren,
                               token_type::e_semi_colon});
         table.set_production(
            {key, token_type::e_break},
            {token_type::e_break, action::e_break_stmt, token_type::e_semi_colon});
         table.set_production(
            {key, token_type::e_continue},
            {token_type::e_continue, action::e_continue_stmt, token_type::e_semi_colon});
      }

      // <StatementList>
      {
         symbol_array common{grammar_type::statement, grammar_type::statement_list};
         const auto key = grammar_type::statement_list;
         table.set_production({key, token_type::e_id}, common);
         table.set_production({key, token_type::e_right_brace}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_if}, common);
         table.set_production({key, token_type::e_while}, common);
         table.set_production({key, token_type::e_read}, common);
         table.set_production({key, token_type::e_write}, common);
         table.set_production({key, token_type::e_return}, common);
         table.set_production({key, token_type::e_break}, common);
         table.set_production({key, token_type::e_continue}, common);
      }

      // <Term>
      {
         symbol_array common{grammar_type::factor, grammar_type::term_tail};
         table.set_production({grammar_type::term, token_type::e_plus}, common);
         table.set_production({grammar_type::term, token_type::e_minus}, common);
         table.set_production({grammar_type::term, token_type::e_id}, common);
         table.set_production({grammar_type::term, token_type::e_integer_lit}, common);
         table.set_production({grammar_type::term, token_type::e_float_lit}, common);
         table.set_production({grammar_type::term, token_type::e_str_lit}, common);
         table.set_production({grammar_type::term, token_type::e_left_paren}, common);
         table.set_production({grammar_type::term, token_type::e_not}, common);
         table.set_production({grammar_type::term, token_type::e_qmark}, common);
      }

      // <TermTail>
      {
         symbol_array common{grammar_type::mult_op, action::e_id_decl, grammar_type::factor,
                             action::e_mult_op, grammar_type::term_tail};
         const auto key = grammar_type::term_tail;
         table.set_production({key, token_type::e_plus}, epsilon);
         table.set_production({key, token_type::e_minus}, epsilon);
         table.set_production({key, token_type::e_or}, epsilon);
         table.set_production({key, token_type::e_comma}, epsilon);
         table.set_production({key, token_type::e_right_square}, epsilon);
         table.set_production({key, token_type::e_semi_colon}, epsilon);
         table.set_production({key, token_type::e_right_paren}, epsilon);
         table.set_production({key, token_type::e_colon}, epsilon);
         table.set_production({key, token_type::e_mult}, common);
         table.set_production({key, token_type::e_div}, common);
         table.set_production({key, token_type::e_and}, common);
         table.set_production({key, token_type::e_equal}, epsilon);
         table.set_production({key, token_type::e_not_equal}, epsilon);
         table.set_production({key, token_type::e_less_than}, epsilon);
         table.set_production({key, token_type::e_greater_thane}, epsilon);
         table.set_production({key, token_type::e_less_equal_than}, epsilon);
         table.set_production({key, token_type::e_greater_equal_than}, epsilon);
      }

      // <Type>
      {
         table.set_production({grammar_type::type, token_type::e_id}, {token_type::e_id});
         table.set_production({grammar_type::type, token_type::e_integer}, {token_type::e_integer});
         table.set_production({grammar_type::type, token_type::e_float}, {token_type::e_float});
         table.set_production({grammar_type::type, token_type::e_string}, {token_type::e_string});
      }

      // <VarDecl>
      {
         symbol_array common{
            grammar_type::type,      action::e_type_decl,           token_type::e_id,
            action::e_id_decl,       grammar_type::array_size_rept, action::e_compound_array_decl,
            token_type::e_semi_colon};
         table.set_production({grammar_type::var_decl, token_type::e_id}, common);
         table.set_production({grammar_type::var_decl, token_type::e_integer}, common);
         table.set_production({grammar_type::var_decl, token_type::e_float}, common);
         table.set_production({grammar_type::var_decl, token_type::e_string}, common);
      }

      // <VarDeclRep>
      {
         symbol_array common{grammar_type::var_decl, action::e_variable_decl,
                             grammar_type::var_decl_rep};
         const auto key = grammar_type::var_decl_rep;
         table.set_production({key, token_type::e_id}, common);
         table.set_production({key, token_type::e_right_brace}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_integer}, common);
         table.set_production({key, token_type::e_float}, common);
         table.set_production({key, token_type::e_string}, common);
      }

      // <Variable>
      {
         table.set_production({grammar_type::variable, token_type::e_id},
                              {token_type::e_id, action::e_id_decl, grammar_type::variable_idnest});
      }

      // <VariableIdnest>
      {
         symbol_array common{grammar_type::indice_rep, action::e_compound_array_index_access_decl,
                             action::e_var_expr, grammar_type::variable_idnest_tail};
         table.set_production({grammar_type::variable_idnest, token_type::e_left_square}, common);
         table.set_production({grammar_type::variable_idnest, token_type::e_right_paren}, common);
         table.set_production({grammar_type::variable_idnest, token_type::e_dot}, common);
      }

      // <VariableIdnestTail>
      {
         const auto key = grammar_type::variable_idnest_tail;
         table.set_production({key, token_type::e_right_paren}, epsilon);
         table.set_production({key, token_type::e_dot},
                              {token_type::e_dot, token_type::e_id, action::e_id_decl,
                               grammar_type::variable_idnest});
      }

      // <Visibility>
      {
         const auto key = grammar_type::visibility;
         table.set_production({key, token_type::e_id}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_func}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_integer}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_float}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_string}, {epsilon[0], action::e_epsilon});
         table.set_production({key, token_type::e_public},
                              {token_type::e_public, action::e_visibily_decl});
         table.set_production({key, token_type::e_private},
                              {token_type::e_private, action::e_visibily_decl});
      }

      return table;
   }
} // namespace fr::grammar
