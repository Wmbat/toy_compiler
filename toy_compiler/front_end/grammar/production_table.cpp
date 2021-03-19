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
      using namespace front;

      production_table table{};
      symbol_array epsilon = {token_type::epsilon};

      // <AddOp>
      {
         const auto key = sem::grammar_type::add_op;
         table.set_production({key, token_type::plus}, {token_type::plus});
         table.set_production({key, token_type::minus}, {token_type::minus});
         table.set_production({key, token_type::or_op}, {token_type::or_op});
      }

      // <aParams>
      {
         symbol_array common{sem::grammar_type::expr, sem::grammar_type::a_params_tail};

         const auto key = sem::grammar_type::a_params;
         table.set_production({key, token_type::plus}, common);
         table.set_production({key, token_type::minus}, common);
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::integer_lit}, common);
         table.set_production({key, token_type::float_lit}, common);
         table.set_production({key, token_type::str_lit}, common);
         table.set_production({key, token_type::left_paren}, common);
         table.set_production({key, token_type::right_paren}, {token_type::epsilon});
         table.set_production({key, token_type::not_op}, common);
         table.set_production({key, token_type::qmark}, common);
      }

      // <aParamsTail>
      {
         const auto key = sem::grammar_type::a_params_tail;
         table.set_production(
            {key, token_type::comma},
            {token_type::comma, sem::grammar_type::expr, sem::grammar_type::a_params_tail});
         table.set_production({key, token_type::right_paren}, epsilon);
      }

      // <ArithExpr>
      {
         symbol_array common{sem::grammar_type::term, sem::grammar_type::arith_expr_tail};
         const auto key = sem::grammar_type::arith_expr;
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
         symbol_array common{sem::grammar_type::add_op, sem::grammar_type::term,
                             sem::grammar_type::arith_expr_tail};
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
                               token_type::right_square, sem::grammar_type::array_size_rept});
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
         table.set_production(
            {sem::grammar_type::class_decl, token_type::id_class},
            {token_type::id_class, front::sem::action::location_decl, token_type::id,
             front::sem::action::id_decl, sem::grammar_type::inherit,
             front::sem::action::compound_inheritance_decl, token_type::left_brace,
             sem::grammar_type::class_decl_body, front::sem::action::compound_member_decl,
             token_type::right_brace, token_type::semi_colon, front::sem::action::class_decl,
             sem::grammar_type::class_decl});
         table.set_production({sem::grammar_type::class_decl, token_type::id_func},
                              {token_type::epsilon, front::sem::action::epsilon});
         table.set_production({sem::grammar_type::class_decl, token_type::id_main},
                              {token_type::epsilon, front::sem::action::epsilon});
      }

      // <ClassDeclBody>
      {
         symbol_array common{sem::grammar_type::visibility, sem::grammar_type::member_decl,
                             front::sem::action::member_decl, sem::grammar_type::class_decl_body};

         const auto key = sem::grammar_type::class_decl_body;
         table.set_production({key, token_type::id}, common);
         table.set_production({key, token_type::right_brace},
                              {epsilon[0], front::sem::action::epsilon});
         table.set_production({key, token_type::id_func}, common);
         table.set_production({key, token_type::id_integer}, common);
         table.set_production({key, token_type::id_float}, common);
         table.set_production({key, token_type::id_string}, common);
         table.set_production({key, token_type::id_public}, common);
         table.set_production({key, token_type::id_private}, common);
      }

      // <ClassMethod>
      {
         table.set_production({sem::grammar_type::class_method, token_type::double_colon},
                              {token_type::double_colon, token_type::id});
         table.set_production({sem::grammar_type::class_method, token_type::left_paren}, epsilon);
      }

      // <Expr>
      {
         symbol_array common{sem::grammar_type::arith_expr, sem::grammar_type::expr_tail};
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
         symbol_array common{sem::grammar_type::relop, sem::grammar_type::arith_expr};
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
         table.set_production({key, token_type::float_lit}, {token_type::float_lit});
         table.set_production({key, token_type::integer_lit}, {token_type::integer_lit});
         table.set_production({key, token_type::str_lit}, {token_type::str_lit});
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
         symbol_array common{sem::grammar_type::type, token_type::id,
                             sem::grammar_type::array_size_rept, sem::grammar_type::f_params_tail};
         table.set_production({sem::grammar_type::f_params, token_type::id}, common);
         table.set_production({sem::grammar_type::f_params, token_type::right_paren}, epsilon);
         table.set_production({sem::grammar_type::f_params, token_type::id_integer}, common);
         table.set_production({sem::grammar_type::f_params, token_type::id_float}, common);
         table.set_production({sem::grammar_type::f_params, token_type::id_string}, common);
      }

      // <fParamsTail>
      {
         table.set_production({sem::grammar_type::f_params_tail, token_type::comma},
                              {token_type::comma, sem::grammar_type::type, token_type::id,
                               sem::grammar_type::array_size_rept,
                               sem::grammar_type::f_params_tail});
         table.set_production({sem::grammar_type::f_params_tail, token_type::right_paren}, epsilon);
      }

      // <FuncBody>
      {
         table.set_production({sem::grammar_type::func_body, token_type::left_brace},
                              {token_type::left_brace, sem::grammar_type::block_variable_decl,
                               sem::grammar_type::statement_list, token_type::right_brace});
      }

      {
         table.set_production({sem::grammar_type::func_decl, token_type::id_func},
                              {token_type::id_func, token_type::id, token_type::left_paren,
                               sem::grammar_type::f_params, token_type::right_paren,
                               token_type::colon, sem::grammar_type::func_decl_tail,
                               token_type::semi_colon});
      }

      // <FuncDeclTail>
      {
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id},
                              {sem::grammar_type::type});
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id_void},
                              {token_type::id_void});
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id_integer},
                              {sem::grammar_type::type});
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id_float},
                              {sem::grammar_type::type});
         table.set_production({sem::grammar_type::func_decl_tail, token_type::id_string},
                              {sem::grammar_type::type});
      }

      // <FuncDef>
      {
         table.set_production({sem::grammar_type::func_def, token_type::id_func},
                              {sem::grammar_type::function, sem::grammar_type::func_def});
         table.set_production({sem::grammar_type::func_def, token_type::id_main}, epsilon);
      }

      // <FuncHead>
      {
         table.set_production({sem::grammar_type::func_head, token_type::id_func},
                              {token_type::id_func, token_type::id, sem::grammar_type::class_method,
                               token_type::left_paren, sem::grammar_type::f_params,
                               token_type::right_paren, token_type::colon,
                               sem::grammar_type::func_decl_tail});
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
                              {sem::grammar_type::func_head, sem::grammar_type::func_body});
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
         table.set_production({sem::grammar_type::inherit, token_type::left_brace},
                              {epsilon[0], front::sem::action::epsilon});
         table.set_production({sem::grammar_type::inherit, token_type::id_inherits},
                              {token_type::id_inherits, token_type::id,
                               front::sem::action::inheritance_decl, sem::grammar_type::nested_id});
      }

      // <IntNum>
      {
         table.set_production({sem::grammar_type::int_num, token_type::integer_lit},
                              {token_type::integer_lit});
         table.set_production({sem::grammar_type::int_num, token_type::right_square}, epsilon);
      }

      //////////////////// HERE

      // <MemberDecl>
      {
         table.set_production({sem::grammar_type::member_decl, token_type::id},
                              {sem::grammar_type::var_decl});
         table.set_production({sem::grammar_type::member_decl, token_type::id_string},
                              {sem::grammar_type::var_decl});
         table.set_production({sem::grammar_type::member_decl, token_type::id_float},
                              {sem::grammar_type::var_decl});
         table.set_production({sem::grammar_type::member_decl, token_type::id_integer},
                              {sem::grammar_type::var_decl});
         table.set_production({sem::grammar_type::member_decl, token_type::id_func},
                              {sem::grammar_type::func_decl});
      }

      // <MethodBodyVar>
      {
         table.set_production({sem::grammar_type::block_variable_decl, token_type::id}, epsilon);
         table.set_production({sem::grammar_type::block_variable_decl, token_type::id_continue},
                              epsilon);
         table.set_production({sem::grammar_type::block_variable_decl, token_type::id_break},
                              epsilon);
         table.set_production({sem::grammar_type::block_variable_decl, token_type::id_return},
                              epsilon);
         table.set_production({sem::grammar_type::block_variable_decl, token_type::id_write},
                              epsilon);
         table.set_production({sem::grammar_type::block_variable_decl, token_type::id_read},
                              epsilon);
         table.set_production({sem::grammar_type::block_variable_decl, token_type::id_while},
                              epsilon);
         table.set_production({sem::grammar_type::block_variable_decl, token_type::id_if}, epsilon);
         table.set_production({sem::grammar_type::block_variable_decl, token_type::id_var},
                              {token_type::id_var, token_type::left_brace,
                               sem::grammar_type::var_decl_rep, token_type::right_brace});
         table.set_production({sem::grammar_type::block_variable_decl, token_type::right_brace},
                              epsilon);
      }

      // <MultOp>
      {
         table.set_production({sem::grammar_type::mult_op, token_type::and_op},
                              {token_type::and_op});
         table.set_production({sem::grammar_type::mult_op, token_type::div}, {token_type::div});
         table.set_production({sem::grammar_type::mult_op, token_type::mult}, {token_type::mult});
      }

      // <NestedId>
      {
         table.set_production({sem::grammar_type::nested_id, token_type::left_brace},
                              {epsilon[0], front::sem::action::epsilon});
         table.set_production({sem::grammar_type::nested_id, token_type::comma},
                              {token_type::comma, token_type::id,
                               front::sem::action::inheritance_decl, sem::grammar_type::nested_id});
      }

      // <Prog>
      {
         symbol_array common{sem::grammar_type::class_decl, front::sem::action::compound_class_decl,
                             sem::grammar_type::func_def, token_type::id_main,
                             sem::grammar_type::func_body};
         table.set_production({sem::grammar_type::prog, token_type::id_main}, common);
         table.set_production({sem::grammar_type::prog, token_type::id_func}, common);
         table.set_production({sem::grammar_type::prog, token_type::id_class}, common);
      }

      // <RelOp>
      {
         table.set_production({sem::grammar_type::relop, token_type::greater_equal_than},
                              {token_type::greater_equal_than});
         table.set_production({sem::grammar_type::relop, token_type::less_equal_than},
                              {token_type::less_equal_than});
         table.set_production({sem::grammar_type::relop, token_type::greater_than},
                              {token_type::greater_than});
         table.set_production({sem::grammar_type::relop, token_type::less_than},
                              {token_type::less_than});
         table.set_production({sem::grammar_type::relop, token_type::not_equal},
                              {token_type::not_equal});
         table.set_production({sem::grammar_type::relop, token_type::equal}, {token_type::equal});
      }

      // <Sign>
      {
         table.set_production({sem::grammar_type::sign, token_type::plus}, {token_type::plus});
         table.set_production({sem::grammar_type::sign, token_type::minus}, {token_type::minus});
      }

      // <Start>
      {
         symbol_array common{sem::grammar_type::prog, front::sem::action::translation_unit};
         table.set_production({sem::grammar_type::start, token_type::id_main}, common);
         table.set_production({sem::grammar_type::start, token_type::id_func}, common);
         table.set_production({sem::grammar_type::start, token_type::id_class}, common);
      }

      // <StatBlock>
      {
         symbol_array common{sem::grammar_type::statement};
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
         table.set_production(
            {sem::grammar_type::stat_block, token_type::left_brace},
            {token_type::left_brace, sem::grammar_type::statement_list, token_type::right_brace});
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
                               token_type::semi_colon});
         table.set_production({key, token_type::id_while},
                              {token_type::id_while, token_type::left_paren,
                               sem::grammar_type::expr, token_type::right_paren,
                               sem::grammar_type::stat_block, token_type::semi_colon});
         table.set_production({key, token_type::id_read},
                              {token_type::id_read, token_type::left_paren,
                               sem::grammar_type::variable, token_type::right_paren,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_write},
                              {token_type::id_write, token_type::left_paren,
                               sem::grammar_type::expr, token_type::right_paren,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_return},
                              {token_type::id_return, token_type::left_paren,
                               sem::grammar_type::expr, token_type::right_paren,
                               token_type::semi_colon});
         table.set_production({key, token_type::id_break},
                              {token_type::id_break, token_type::semi_colon});
         table.set_production({key, token_type::id_continue},
                              {token_type::id_continue, token_type::semi_colon});
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
         symbol_array common{sem::grammar_type::factor, sem::grammar_type::term_tail};
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
         symbol_array common{sem::grammar_type::mult_op, sem::grammar_type::factor,
                             sem::grammar_type::term_tail};
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
         symbol_array common{sem::grammar_type::type, token_type::id,
                             sem::grammar_type::array_size_rept, token_type::semi_colon};
         table.set_production({sem::grammar_type::var_decl, token_type::id}, common);
         table.set_production({sem::grammar_type::var_decl, token_type::id_integer}, common);
         table.set_production({sem::grammar_type::var_decl, token_type::id_float}, common);
         table.set_production({sem::grammar_type::var_decl, token_type::id_string}, common);
      }

      // <VarDeclRep>
      {
         symbol_array common{sem::grammar_type::var_decl, sem::grammar_type::var_decl_rep};
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
         const auto key = sem::grammar_type::visibility;
         table.set_production({key, token_type::id}, {epsilon[0], front::sem::action::epsilon});
         table.set_production({key, token_type::id_func},
                              {epsilon[0], front::sem::action::epsilon});
         table.set_production({key, token_type::id_integer},
                              {epsilon[0], front::sem::action::epsilon});
         table.set_production({key, token_type::id_float},
                              {epsilon[0], front::sem::action::epsilon});
         table.set_production({key, token_type::id_string},
                              {epsilon[0], front::sem::action::epsilon});
         table.set_production({key, token_type::id_public},
                              {token_type::id_public, front::sem::action::visibily_decl});
         table.set_production({key, token_type::id_private},
                              {token_type::id_private, front::sem::action::visibily_decl});
      }

      return table;
   }
} // namespace fr::grammar
