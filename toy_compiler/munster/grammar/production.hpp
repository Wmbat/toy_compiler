/**
 * @file rule.hpp
 * @brief Contains all element relating to the `grammar::rule` class
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

#pragma once

#include <toy_compiler/munster/grammar/grammar_type.hpp>
#include <toy_compiler/munster/grammar/symbol.hpp>
#include <toy_compiler/munster/grammar/token_type.hpp>

#include <toy_compiler/util/strong_type.hpp>

#include <fmt/ranges.h>

#include <memory>
#include <utility>

namespace munster::grammar
{
   /**
    * @brief shorthand to designate a dynamic array containing `grammar::symbol`s with a small
    * buffer of 5 elements
    */
   using symbol_array = std::vector<symbol>; // NOLINT

   using nullable_t = util::strong_type<bool, struct nullable, util::equatable>;

   /**
    * @brief A simple class to hold the information of a grammar rule.
    *
    * The purpose of this class is to simplify the created of rules to use in the parsing table. The
    * rules are encoded using the following rule: **E -> aT`F**.
    */
   class production
   {
   public:
      production() = default;
      /**
       * @brief Create a rule
       *
       * @param[in] start The left hand `sem::grammar_type` to use as the starting point of the
       * `grammar::rule`.
       * @param[in] tail The resulting `grammar::symbol`s derived from the `start`.
       * @param[in] is_nullable Tell whether the rule may be null. **False** by default.
       */
      production(grammar_type start, symbol_array tail, nullable_t is_nullable = nullable_t{false});

      auto operator==(const production& other) const -> bool = default;

      /**
       * @brief Access the start of the `grammar::rule`
       */
      [[nodiscard]] auto start() const -> grammar_type;

      /**
       * @brief Access the tail of the `grammar::rule`
       */
      [[nodiscard]] auto tail() const -> const symbol_array&;
      /**
       * @brief Access the tail of the `grammar::rule`
       */
      auto tail() -> symbol_array&;

      /**
       * @brief Check if the `grammar::rule` is nullable
       */
      [[nodiscard]] auto nullable() const noexcept -> bool;

   private:
      grammar_type m_start = grammar_type::max_size;
      symbol_array m_symbols{};
      nullable_t m_is_nullable{false};
   };

   namespace sets
   {
      static const std::array<production, 54> first{{
         {grammar_type::add_op, {token_type::e_or, token_type::e_plus, token_type::e_minus}},
         {grammar_type::a_params,
          {token_type::e_plus, token_type::e_minus, token_type::e_id, token_type::e_integer_lit,
           token_type::e_float_lit, token_type::e_str_lit, token_type::e_left_paren,
           token_type::e_qmark, token_type::e_not},
          nullable_t{true}},
         {grammar_type::a_params_tail, {token_type::e_comma}, nullable_t{true}},
         {grammar_type::arith_expr,
          {token_type::e_plus, token_type::e_minus, token_type::e_id, token_type::e_integer_lit,
           token_type::e_float_lit, token_type::e_str_lit, token_type::e_left_paren,
           token_type::e_qmark, token_type::e_not}},
         {grammar_type::arith_expr_tail,
          {token_type::e_plus, token_type::e_minus, token_type::e_or},
          nullable_t{true}},
         {grammar_type::array_size_rept, {token_type::e_left_square}, nullable_t{true}},
         {grammar_type::assign_op, {token_type::e_assign}},
         {grammar_type::assign_stat_tail, {token_type::e_assign}},
         {grammar_type::class_decl, {token_type::e_class}, nullable_t{true}},
         {grammar_type::class_decl_body,
          {token_type::e_public, token_type::e_id, token_type::e_func, token_type::e_private,
           token_type::e_string, token_type::e_integer, token_type::e_float},
          nullable_t{true}},
         {grammar_type::class_method, {token_type::e_double_colon}, nullable_t{true}},
         {grammar_type::expr,
          {token_type::e_plus, token_type::e_minus, token_type::e_id, token_type::e_integer_lit,
           token_type::e_float_lit, token_type::e_str_lit, token_type::e_left_paren,
           token_type::e_not}},
         {grammar_type::expr_tail,
          {token_type::e_equal, token_type::e_not_equal, token_type::e_less_than,
           token_type::e_greater_thane, token_type::e_less_equal_than,
           token_type::e_greater_equal_than},
          nullable_t{true}},
         {grammar_type::factor,
          {token_type::e_plus, token_type::e_minus, token_type::e_id, token_type::e_integer_lit,
           token_type::e_float_lit, token_type::e_str_lit, token_type::e_left_paren,
           token_type::e_qmark, token_type::e_not}},
         {grammar_type::f_params,
          {token_type::e_id, token_type::e_string, token_type::e_integer, token_type::e_float},
          nullable_t{true}},
         {grammar_type::f_params_tail, {token_type::e_comma}, nullable_t{true}},
         {grammar_type::func_body, {token_type::e_left_brace}},
         {grammar_type::func_decl, {token_type::e_func}},
         {grammar_type::func_decl_tail,
          {token_type::e_id, token_type::e_string, token_type::e_integer, token_type::e_float,
           token_type::e_void}},
         {grammar_type::func_def, {token_type::e_func}, nullable_t{true}},
         {grammar_type::func_head, {token_type::e_func}},
         {grammar_type::func_or_assign_stat, {token_type::e_id}},
         {grammar_type::func_or_assign_stat_idnest,
          {token_type::e_left_paren, token_type::e_left_square, token_type::e_assign,
           token_type::e_dot}},
         {grammar_type::func_or_assign_stat_idnest_func_tail,
          {token_type::e_dot},
          nullable_t{true}},
         {grammar_type::func_or_assign_stat_idnest_var_tail,
          {token_type::e_assign, token_type::e_dot}},
         {grammar_type::func_or_var, {token_type::e_id}},
         {grammar_type::func_or_var_idnest,
          {token_type::e_left_paren, token_type::e_left_square, token_type::e_dot},
          nullable_t{true}},
         {grammar_type::func_or_var_idnest_tail, {token_type::e_dot}, nullable_t{true}},
         {grammar_type::func_stat_tail,
          {token_type::e_left_paren, token_type::e_dot, token_type::e_left_square}},
         {grammar_type::func_stat_tail_idnest, {token_type::e_dot}, nullable_t{true}},
         {grammar_type::function, {token_type::e_func}},
         {grammar_type::indice_rep, {token_type::e_left_square}, nullable_t{true}},
         {grammar_type::inherit, {token_type::e_inherits}, nullable_t{true}},
         {grammar_type::int_num, {token_type::e_integer_lit}, nullable_t{true}},
         {grammar_type::member_decl,
          {token_type::e_id, token_type::e_func, token_type::e_string, token_type::e_integer,
           token_type::e_float}},
         {grammar_type::block_variable_decl, {token_type::e_var}, nullable_t{true}},
         {grammar_type::mult_op, {token_type::e_mult, token_type::e_and, token_type::e_div}},
         {grammar_type::nested_id, {token_type::e_comma}, nullable_t{true}},
         {grammar_type::prog, {token_type::e_main, token_type::e_class, token_type::e_func}},
         {grammar_type::relop,
          {token_type::e_less_equal_than, token_type::e_greater_thane, token_type::e_not_equal,
           token_type::e_less_than, token_type::e_greater_equal_than, token_type::e_equal}},
         {grammar_type::sign, {token_type::e_plus, token_type::e_minus}},
         {grammar_type::start, {token_type::e_main, token_type::e_class, token_type::e_func}},
         {grammar_type::stat_block,
          {token_type::e_left_brace, token_type::e_write, token_type::e_id, token_type::e_if,
           token_type::e_read, token_type::e_while, token_type::e_break, token_type::e_continue},
          nullable_t{true}},
         {grammar_type::statement,
          {token_type::e_id, token_type::e_while, token_type::e_write, token_type::e_return,
           token_type::e_if, token_type::e_read, token_type::e_while, token_type::e_break,
           token_type::e_continue}},
         {grammar_type::statement_list,
          {token_type::e_id, token_type::e_while, token_type::e_write, token_type::e_return,
           token_type::e_if, token_type::e_read, token_type::e_while, token_type::e_break,
           token_type::e_continue},
          nullable_t{true}},
         {grammar_type::term,
          {token_type::e_float_lit, token_type::e_integer_lit, token_type::e_str_lit,
           token_type::e_id, token_type::e_left_paren, token_type::e_qmark, token_type::e_plus,
           token_type::e_minus, token_type::e_not}},
         {grammar_type::term_tail,
          {token_type::e_mult, token_type::e_and, token_type::e_div},
          nullable_t{true}},
         {grammar_type::type,
          {token_type::e_id, token_type::e_string, token_type::e_integer, token_type::e_float}},
         {grammar_type::var_decl,
          {token_type::e_id, token_type::e_string, token_type::e_integer, token_type::e_float}},
         {grammar_type::var_decl_rep,
          {token_type::e_id, token_type::e_string, token_type::e_integer, token_type::e_float}},
         {grammar_type::variable, {token_type::e_id}},
         {grammar_type::variable_idnest_tail, {token_type::e_dot}, nullable_t{true}},
         {grammar_type::variable_idnest,
          {token_type::e_dot, token_type::e_left_square},
          nullable_t{true}},
         {grammar_type::visibility,
          {token_type::e_public, token_type::e_private},
          nullable_t{true}},
      }};

      static const std::array<production, 54> follow{{
         {grammar_type::add_op,
          {token_type::e_plus, token_type::e_minus, token_type::e_id, token_type::e_integer_lit,
           token_type::e_float_lit, token_type::e_str_lit, token_type::e_left_paren,
           token_type::e_not, token_type::e_qmark}},
         {grammar_type::a_params, {token_type::e_right_paren}, nullable_t{true}},
         {grammar_type::a_params_tail, {token_type::e_right_paren}, nullable_t{true}},
         {grammar_type::arith_expr,
          {token_type::e_comma, token_type::e_right_square, token_type::e_semi_colon,
           token_type::e_right_paren, token_type::e_colon, token_type::e_equal,
           token_type::e_not_equal, token_type::e_less_than, token_type::e_greater_thane,
           token_type::e_less_equal_than, token_type::e_greater_equal_than}},
         {grammar_type::arith_expr_tail,
          {token_type::e_comma, token_type::e_right_square, token_type::e_semi_colon,
           token_type::e_right_paren, token_type::e_colon, token_type::e_equal,
           token_type::e_not_equal, token_type::e_less_than, token_type::e_greater_thane,
           token_type::e_less_equal_than, token_type::e_greater_equal_than},
          nullable_t{true}},
         {grammar_type::array_size_rept,
          {token_type::e_comma, token_type::e_semi_colon, token_type::e_right_paren},
          nullable_t{true}},
         {grammar_type::assign_op,
          {token_type::e_plus, token_type::e_minus, token_type::e_id, token_type::e_integer_lit,
           token_type::e_float_lit, token_type::e_str_lit, token_type::e_left_paren,
           token_type::e_not, token_type::e_qmark}},
         {grammar_type::assign_stat_tail, {token_type::e_semi_colon}},
         {grammar_type::class_decl, {token_type::e_func, token_type::e_main}, nullable_t{true}},
         {grammar_type::class_decl_body, {token_type::e_left_brace}, nullable_t{true}},
         {grammar_type::class_method, {token_type::e_left_paren}, nullable_t{true}},
         {grammar_type::expr,
          {token_type::e_comma, token_type::e_right_square, token_type::e_semi_colon,
           token_type::e_right_paren, token_type::e_colon}},
         {grammar_type::expr_tail,
          {token_type::e_comma, token_type::e_right_square, token_type::e_semi_colon,
           token_type::e_right_paren, token_type::e_colon},
          nullable_t{true}},
         {grammar_type::factor,
          {token_type::e_plus, token_type::e_minus, token_type::e_comma, token_type::e_or,
           token_type::e_right_square, token_type::e_semi_colon, token_type::e_right_paren,
           token_type::e_colon, token_type::e_mult, token_type::e_div, token_type::e_and,
           token_type::e_equal, token_type::e_not_equal, token_type::e_less_than,
           token_type::e_greater_thane, token_type::e_less_equal_than,
           token_type::e_greater_equal_than}},
         {grammar_type::f_params, {token_type::e_right_paren}, nullable_t{true}},
         {grammar_type::f_params_tail, {token_type::e_right_paren}, nullable_t{true}},
         {grammar_type::func_body, {token_type::e_func, token_type::e_main}},
         {grammar_type::func_decl,
          {token_type::e_id, token_type::e_right_brace, token_type::e_func, token_type::e_integer,
           token_type::e_float, token_type::e_string, token_type::e_public, token_type::e_private}},
         {grammar_type::func_decl_tail, {token_type::e_left_brace, token_type::e_semi_colon}},
         {grammar_type::func_def, {token_type::e_main}, nullable_t{true}},
         {grammar_type::func_head, {token_type::e_left_brace}},
         {grammar_type::func_or_assign_stat, {token_type::e_semi_colon}},
         {grammar_type::func_or_assign_stat_idnest, {token_type::e_semi_colon}},
         {grammar_type::func_or_assign_stat_idnest_func_tail,
          {token_type::e_semi_colon},
          nullable_t{true}},
         {grammar_type::func_or_assign_stat_idnest_var_tail, {token_type::e_semi_colon}},
         {grammar_type::func_or_var,
          {token_type::e_plus, token_type::e_minus, token_type::e_comma, token_type::e_or,
           token_type::e_right_square, token_type::e_semi_colon, token_type::e_right_paren,
           token_type::e_colon, token_type::e_mult, token_type::e_div, token_type::e_and,
           token_type::e_equal, token_type::e_not_equal, token_type::e_less_than,
           token_type::e_greater_thane, token_type::e_less_equal_than,
           token_type::e_greater_equal_than}},
         {grammar_type::func_or_var_idnest,
          {token_type::e_plus, token_type::e_minus, token_type::e_comma, token_type::e_or,
           token_type::e_right_square, token_type::e_semi_colon, token_type::e_right_paren,
           token_type::e_colon, token_type::e_mult, token_type::e_div, token_type::e_and,
           token_type::e_equal, token_type::e_not_equal, token_type::e_less_than,
           token_type::e_greater_thane, token_type::e_less_equal_than,
           token_type::e_greater_equal_than},
          nullable_t{true}},
         {grammar_type::func_or_var_idnest_tail,
          {token_type::e_plus, token_type::e_minus, token_type::e_comma, token_type::e_or,
           token_type::e_right_square, token_type::e_semi_colon, token_type::e_right_paren,
           token_type::e_colon, token_type::e_mult, token_type::e_div, token_type::e_and,
           token_type::e_equal, token_type::e_not_equal, token_type::e_less_than,
           token_type::e_greater_thane, token_type::e_less_equal_than,
           token_type::e_greater_equal_than},
          nullable_t{true}},
         {grammar_type::func_stat_tail, {token_type::e_semi_colon}},
         {grammar_type::func_stat_tail_idnest, {token_type::e_semi_colon}, nullable_t{true}},
         {grammar_type::function, {token_type::e_func, token_type::e_main}},
         {grammar_type::indice_rep,
          {token_type::e_plus, token_type::e_minus, token_type::e_comma, token_type::e_or,
           token_type::e_right_square, token_type::e_assign, token_type::e_semi_colon,
           token_type::e_right_paren, token_type::e_colon, token_type::e_mult, token_type::e_div,
           token_type::e_and, token_type::e_equal, token_type::e_not_equal, token_type::e_less_than,
           token_type::e_greater_thane, token_type::e_less_equal_than,
           token_type::e_greater_equal_than},
          nullable_t{true}},
         {grammar_type::inherit, {token_type::e_left_brace}, nullable_t{true}},
         {grammar_type::int_num, {token_type::e_right_square}, nullable_t{true}},
         {grammar_type::member_decl,
          {token_type::e_id, token_type::e_right_brace, token_type::e_func, token_type::e_integer,
           token_type::e_float, token_type::e_string, token_type::e_public, token_type::e_private}},
         {grammar_type::block_variable_decl,
          {token_type::e_id, token_type::e_right_brace, token_type::e_if, token_type::e_while,
           token_type::e_read, token_type::e_write, token_type::e_return, token_type::e_break,
           token_type::e_continue},
          nullable_t{true}},
         {grammar_type::mult_op,
          {token_type::e_plus, token_type::e_minus, token_type::e_id, token_type::e_integer_lit,
           token_type::e_float_lit, token_type::e_str_lit, token_type::e_left_paren,
           token_type::e_not, token_type::e_qmark}},
         {grammar_type::nested_id, {token_type::e_left_brace}, nullable_t{true}},
         {grammar_type::prog, {}},
         {grammar_type::relop,
          {token_type::e_plus, token_type::e_minus, token_type::e_id, token_type::e_integer_lit,
           token_type::e_float_lit, token_type::e_str_lit, token_type::e_left_paren,
           token_type::e_not, token_type::e_qmark}},
         {grammar_type::sign,
          {token_type::e_plus, token_type::e_minus, token_type::e_id, token_type::e_integer_lit,
           token_type::e_float_lit, token_type::e_str_lit, token_type::e_left_paren,
           token_type::e_not, token_type::e_qmark}},
         {grammar_type::start, {}},
         {grammar_type::stat_block,
          {token_type::e_semi_colon, token_type::e_else},
          nullable_t{true}},
         {grammar_type::statement,
          {token_type::e_id, token_type::e_right_brace, token_type::e_semi_colon, token_type::e_if,
           token_type::e_else, token_type::e_while, token_type::e_read, token_type::e_write,
           token_type::e_return, token_type::e_break, token_type::e_continue}},
         {grammar_type::statement_list, {token_type::e_right_brace}, nullable_t{true}},
         {grammar_type::term,
          {token_type::e_plus, token_type::e_minus, token_type::e_or, token_type::e_comma,
           token_type::e_right_square, token_type::e_semi_colon, token_type::e_right_paren,
           token_type::e_colon, token_type::e_equal, token_type::e_not_equal,
           token_type::e_less_than, token_type::e_greater_thane, token_type::e_less_equal_than,
           token_type::e_greater_equal_than}},
         {grammar_type::term_tail,
          {token_type::e_plus, token_type::e_minus, token_type::e_or, token_type::e_comma,
           token_type::e_right_square, token_type::e_semi_colon, token_type::e_right_paren,
           token_type::e_colon, token_type::e_equal, token_type::e_not_equal,
           token_type::e_less_than, token_type::e_greater_thane, token_type::e_less_equal_than,
           token_type::e_greater_equal_than},
          nullable_t{true}},
         {grammar_type::type,
          {token_type::e_id, token_type::e_right_brace, token_type::e_semi_colon}},
         {grammar_type::var_decl,
          {token_type::e_id, token_type::e_right_brace, token_type::e_func, token_type::e_integer,
           token_type::e_float, token_type::e_string, token_type::e_public, token_type::e_private}},
         {grammar_type::var_decl_rep, {token_type::e_id, token_type::e_right_brace}},
         {grammar_type::variable, {token_type::e_right_paren}},
         {grammar_type::variable_idnest, {token_type::e_right_paren}, nullable_t{true}},
         {grammar_type::variable_idnest_tail, {token_type::e_right_paren}, nullable_t{true}},
         {grammar_type::visibility,
          {token_type::e_id, token_type::e_func, token_type::e_integer, token_type::e_float,
           token_type::e_string},
          nullable_t{true}},
      }};
   } // namespace sets
} // namespace munster::grammar

/**
 * @brief A specialization for using the `grammar::rule` class in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<munster::grammar::production>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const munster::grammar::production& r, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{} -> {}", r.start(), r.tail());
   }
};
