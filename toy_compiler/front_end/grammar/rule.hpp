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

#include <toy_compiler/front_end/grammar/grammar_type.hpp>
#include <toy_compiler/front_end/grammar/symbol.hpp>
#include <toy_compiler/front_end/grammar/token_type.hpp>

#include <toy_compiler/util/strong_type.hpp>

#include <fmt/ranges.h>

#include <memory>
#include <utility>

namespace fr::grammar
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
   class rule
   {
   public:
      rule() = default;
      /**
       * @brief Create a rule
       *
       * @param[in] start The left hand `grammar::grammar_type` to use as the starting point of the
       * `grammar::rule`.
       * @param[in] tail The resulting `grammar::symbol`s derived from the `start`.
       * @param[in] is_nullable Tell whether the rule may be null. **False** by default.
       */
      rule(grammar_type start, symbol_array tail, nullable_t is_nullable = nullable_t{false});

      auto operator==(const rule& other) const -> bool = default;

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
      auto nullable() const noexcept -> bool;

   private:
      grammar_type m_start = grammar_type::max_size;
      symbol_array m_symbols{};
      nullable_t m_is_nullable{false};
   };

   namespace sets
   {
      static const std::array<rule, 54> first{{
         {grammar_type::add_op, {token_type::or_op, token_type::plus, token_type::minus}},
         {grammar_type::a_params,
          {token_type::plus, token_type::minus, token_type::id, token_type::integer_lit,
           token_type::float_lit, token_type::str_lit, token_type::left_paren, token_type::qmark,
           token_type::not_op},
          nullable_t{true}},
         {grammar_type::a_params_tail, {token_type::comma}, nullable_t{true}},
         {grammar_type::arith_expr,
          {token_type::plus, token_type::minus, token_type::id, token_type::integer_lit,
           token_type::float_lit, token_type::str_lit, token_type::left_paren, token_type::qmark,
           token_type::not_op}},
         {grammar_type::arith_expr_tail,
          {token_type::plus, token_type::minus, token_type::or_op},
          nullable_t{true}},
         {grammar_type::array_size_rept, {token_type::left_square}, nullable_t{true}},
         {grammar_type::assign_op, {token_type::assign}},
         {grammar_type::assign_stat_tail, {token_type::assign}},
         {grammar_type::class_decl, {token_type::id_class}, nullable_t{true}},
         {grammar_type::class_decl_body,
          {token_type::id_public, token_type::id, token_type::id_func, token_type::id_private,
           token_type::id_string, token_type::id_integer, token_type::id_float},
          nullable_t{true}},
         {grammar_type::class_method, {token_type::double_colon}, nullable_t{true}},
         {grammar_type::expr,
          {token_type::plus, token_type::minus, token_type::id, token_type::integer_lit,
           token_type::float_lit, token_type::str_lit, token_type::left_paren, token_type::not_op}},
         {grammar_type::expr_tail,
          {token_type::equal, token_type::not_equal, token_type::less_than,
           token_type::greater_than, token_type::less_equal_than, token_type::greater_equal_than},
          nullable_t{true}},
         {grammar_type::factor,
          {token_type::plus, token_type::minus, token_type::id, token_type::integer_lit,
           token_type::float_lit, token_type::str_lit, token_type::left_paren, token_type::qmark,
           token_type::not_op}},
         {grammar_type::f_params,
          {token_type::id, token_type::id_string, token_type::id_integer, token_type::id_float},
          nullable_t{true}},
         {grammar_type::f_params_tail, {token_type::comma}, nullable_t{true}},
         {grammar_type::func_body, {token_type::left_brace}},
         {grammar_type::func_decl, {token_type::id_func}},
         {grammar_type::func_decl_tail,
          {token_type::id, token_type::id_string, token_type::id_integer, token_type::id_float,
           token_type::id_void}},
         {grammar_type::func_def, {token_type::id_func}, nullable_t{true}},
         {grammar_type::func_head, {token_type::id_func}},
         {grammar_type::func_or_assign_stat, {token_type::id}},
         {grammar_type::func_or_assign_stat_idnest,
          {token_type::left_paren, token_type::left_square, token_type::assign,
           token_type::period}},
         {grammar_type::func_or_assign_stat_idnest_func_tail,
          {token_type::period},
          nullable_t{true}},
         {grammar_type::func_or_assign_stat_idnest_var_tail,
          {token_type::assign, token_type::period}},
         {grammar_type::func_or_var, {token_type::id}},
         {grammar_type::func_or_var_idnest,
          {token_type::left_paren, token_type::left_square, token_type::period},
          nullable_t{true}},
         {grammar_type::func_or_var_idnest_tail, {token_type::period}, nullable_t{true}},
         {grammar_type::func_stat_tail,
          {token_type::left_paren, token_type::period, token_type::left_square}},
         {grammar_type::func_stat_tail_idnest, {token_type::period}, nullable_t{true}},
         {grammar_type::function, {token_type::id_func}},
         {grammar_type::indice_rep, {token_type::left_square}, nullable_t{true}},
         {grammar_type::inherit, {token_type::id_inherits}, nullable_t{true}},
         {grammar_type::int_num, {token_type::integer_lit}, nullable_t{true}},
         {grammar_type::member_decl,
          {token_type::id, token_type::id_func, token_type::id_string, token_type::id_integer,
           token_type::id_float}},
         {grammar_type::method_body_var, {token_type::id_var}, nullable_t{true}},
         {grammar_type::mult_op, {token_type::mult, token_type::and_op, token_type::div}},
         {grammar_type::nested_id, {token_type::comma}, nullable_t{true}},
         {grammar_type::prog, {token_type::id_main, token_type::id_class, token_type::id_func}},
         {grammar_type::relop,
          {token_type::less_equal_than, token_type::greater_than, token_type::not_equal,
           token_type::less_than, token_type::greater_equal_than, token_type::equal}},
         {grammar_type::sign, {token_type::plus, token_type::minus}},
         {grammar_type::start, {token_type::id_main, token_type::id_class, token_type::id_func}},
         {grammar_type::stat_block,
          {token_type::left_brace, token_type::id_write, token_type::id, token_type::id_if,
           token_type::id_read, token_type::id_while, token_type::id_break,
           token_type::id_continue},
          nullable_t{true}},
         {grammar_type::statement,
          {token_type::id, token_type::id_while, token_type::id_write, token_type::id_return,
           token_type::id_if, token_type::id_read, token_type::id_while, token_type::id_break,
           token_type::id_continue}},
         {grammar_type::statement_list,
          {token_type::id, token_type::id_while, token_type::id_write, token_type::id_return,
           token_type::id_if, token_type::id_read, token_type::id_while, token_type::id_break,
           token_type::id_continue},
          nullable_t{true}},
         {grammar_type::term,
          {token_type::float_lit, token_type::integer_lit, token_type::str_lit, token_type::id,
           token_type::left_paren, token_type::qmark, token_type::plus, token_type::minus,
           token_type::not_op}},
         {grammar_type::term_tail,
          {token_type::mult, token_type::and_op, token_type::div},
          nullable_t{true}},
         {grammar_type::type,
          {token_type::id, token_type::id_string, token_type::id_integer, token_type::id_float}},
         {grammar_type::var_decl,
          {token_type::id, token_type::id_string, token_type::id_integer, token_type::id_float}},
         {grammar_type::var_decl_rep,
          {token_type::id, token_type::id_string, token_type::id_integer, token_type::id_float}},
         {grammar_type::variable, {token_type::id}},
         {grammar_type::variable_idnest_tail, {token_type::period}, nullable_t{true}},
         {grammar_type::variable_idnest,
          {token_type::period, token_type::left_square},
          nullable_t{true}},
         {grammar_type::visibility,
          {token_type::id_public, token_type::id_private},
          nullable_t{true}},
      }};

      static const std::array<rule, 54> follow{{
         {grammar_type::add_op,
          {token_type::plus, token_type::minus, token_type::id, token_type::integer_lit,
           token_type::float_lit, token_type::str_lit, token_type::left_paren, token_type::not_op,
           token_type::qmark}},
         {grammar_type::a_params, {token_type::right_paren}, nullable_t{true}},
         {grammar_type::a_params_tail, {token_type::right_paren}, nullable_t{true}},
         {grammar_type::arith_expr,
          {token_type::comma, token_type::right_square, token_type::semi_colon,
           token_type::right_paren, token_type::colon, token_type::equal, token_type::not_equal,
           token_type::less_than, token_type::greater_than, token_type::less_equal_than,
           token_type::greater_equal_than}},
         {grammar_type::arith_expr_tail,
          {token_type::comma, token_type::right_square, token_type::semi_colon,
           token_type::right_paren, token_type::colon, token_type::equal, token_type::not_equal,
           token_type::less_than, token_type::greater_than, token_type::less_equal_than,
           token_type::greater_equal_than},
          nullable_t{true}},
         {grammar_type::array_size_rept,
          {token_type::comma, token_type::semi_colon, token_type::right_paren},
          nullable_t{true}},
         {grammar_type::assign_op,
          {token_type::plus, token_type::minus, token_type::id, token_type::integer_lit,
           token_type::float_lit, token_type::str_lit, token_type::left_paren, token_type::not_op,
           token_type::qmark}},
         {grammar_type::assign_stat_tail, {token_type::semi_colon}},
         {grammar_type::class_decl, {token_type::id_func, token_type::id_main}, nullable_t{true}},
         {grammar_type::class_decl_body, {token_type::left_brace}, nullable_t{true}},
         {grammar_type::class_method, {token_type::left_paren}, nullable_t{true}},
         {grammar_type::expr,
          {token_type::comma, token_type::right_square, token_type::semi_colon,
           token_type::right_paren, token_type::colon}},
         {grammar_type::expr_tail,
          {token_type::comma, token_type::right_square, token_type::semi_colon,
           token_type::right_paren, token_type::colon},
          nullable_t{true}},
         {grammar_type::factor,
          {token_type::plus, token_type::minus, token_type::comma, token_type::or_op,
           token_type::right_square, token_type::semi_colon, token_type::right_paren,
           token_type::colon, token_type::mult, token_type::div, token_type::and_op,
           token_type::equal, token_type::not_equal, token_type::less_than,
           token_type::greater_than, token_type::less_equal_than, token_type::greater_equal_than}},
         {grammar_type::f_params, {token_type::right_paren}, nullable_t{true}},
         {grammar_type::f_params_tail, {token_type::right_paren}, nullable_t{true}},
         {grammar_type::func_body, {token_type::id_func, token_type::id_main}},
         {grammar_type::func_decl,
          {token_type::id, token_type::right_brace, token_type::id_func, token_type::id_integer,
           token_type::id_float, token_type::id_string, token_type::id_public,
           token_type::id_private}},
         {grammar_type::func_decl_tail, {token_type::left_brace, token_type::semi_colon}},
         {grammar_type::func_def, {token_type::id_main}, nullable_t{true}},
         {grammar_type::func_head, {token_type::left_brace}},
         {grammar_type::func_or_assign_stat, {token_type::semi_colon}},
         {grammar_type::func_or_assign_stat_idnest, {token_type::semi_colon}},
         {grammar_type::func_or_assign_stat_idnest_func_tail,
          {token_type::semi_colon},
          nullable_t{true}},
         {grammar_type::func_or_assign_stat_idnest_var_tail, {token_type::semi_colon}},
         {grammar_type::func_or_var,
          {token_type::plus, token_type::minus, token_type::comma, token_type::or_op,
           token_type::right_square, token_type::semi_colon, token_type::right_paren,
           token_type::colon, token_type::mult, token_type::div, token_type::and_op,
           token_type::equal, token_type::not_equal, token_type::less_than,
           token_type::greater_than, token_type::less_equal_than, token_type::greater_equal_than}},
         {grammar_type::func_or_var_idnest,
          {token_type::plus, token_type::minus, token_type::comma, token_type::or_op,
           token_type::right_square, token_type::semi_colon, token_type::right_paren,
           token_type::colon, token_type::mult, token_type::div, token_type::and_op,
           token_type::equal, token_type::not_equal, token_type::less_than,
           token_type::greater_than, token_type::less_equal_than, token_type::greater_equal_than},
          nullable_t{true}},
         {grammar_type::func_or_var_idnest_tail,
          {token_type::plus, token_type::minus, token_type::comma, token_type::or_op,
           token_type::right_square, token_type::semi_colon, token_type::right_paren,
           token_type::colon, token_type::mult, token_type::div, token_type::and_op,
           token_type::equal, token_type::not_equal, token_type::less_than,
           token_type::greater_than, token_type::less_equal_than, token_type::greater_equal_than},
          nullable_t{true}},
         {grammar_type::func_stat_tail, {token_type::semi_colon}},
         {grammar_type::func_stat_tail_idnest, {token_type::semi_colon}, nullable_t{true}},
         {grammar_type::function, {token_type::id_func, token_type::id_main}},
         {grammar_type::indice_rep,
          {token_type::plus, token_type::minus, token_type::comma, token_type::or_op,
           token_type::right_square, token_type::assign, token_type::semi_colon,
           token_type::right_paren, token_type::colon, token_type::mult, token_type::div,
           token_type::and_op, token_type::equal, token_type::not_equal, token_type::less_than,
           token_type::greater_than, token_type::less_equal_than, token_type::greater_equal_than},
          nullable_t{true}},
         {grammar_type::inherit, {token_type::left_brace}, nullable_t{true}},
         {grammar_type::int_num, {token_type::right_square}, nullable_t{true}},
         {grammar_type::member_decl,
          {token_type::id, token_type::right_brace, token_type::id_func, token_type::id_integer,
           token_type::id_float, token_type::id_string, token_type::id_public,
           token_type::id_private}},
         {grammar_type::method_body_var,
          {token_type::id, token_type::right_brace, token_type::id_if, token_type::id_while,
           token_type::id_read, token_type::id_write, token_type::id_return, token_type::id_break,
           token_type::id_continue},
          nullable_t{true}},
         {grammar_type::mult_op,
          {token_type::plus, token_type::minus, token_type::id, token_type::integer_lit,
           token_type::float_lit, token_type::str_lit, token_type::left_paren, token_type::not_op,
           token_type::qmark}},
         {grammar_type::nested_id, {token_type::left_brace}, nullable_t{true}},
         {grammar_type::prog, {}},
         {grammar_type::relop,
          {token_type::plus, token_type::minus, token_type::id, token_type::integer_lit,
           token_type::float_lit, token_type::str_lit, token_type::left_paren, token_type::not_op,
           token_type::qmark}},
         {grammar_type::sign,
          {token_type::plus, token_type::minus, token_type::id, token_type::integer_lit,
           token_type::float_lit, token_type::str_lit, token_type::left_paren, token_type::not_op,
           token_type::qmark}},
         {grammar_type::start, {}},
         {grammar_type::stat_block,
          {token_type::semi_colon, token_type::id_else},
          nullable_t{true}},
         {grammar_type::statement,
          {token_type::id, token_type::right_brace, token_type::semi_colon, token_type::id_if,
           token_type::id_else, token_type::id_while, token_type::id_read, token_type::id_write,
           token_type::id_return, token_type::id_break, token_type::id_continue}},
         {grammar_type::statement_list, {token_type::right_brace}, nullable_t{true}},
         {grammar_type::term,
          {token_type::plus, token_type::minus, token_type::or_op, token_type::comma,
           token_type::right_square, token_type::semi_colon, token_type::right_paren,
           token_type::colon, token_type::equal, token_type::not_equal, token_type::less_than,
           token_type::greater_than, token_type::less_equal_than, token_type::greater_equal_than}},
         {grammar_type::term_tail,
          {token_type::plus, token_type::minus, token_type::or_op, token_type::comma,
           token_type::right_square, token_type::semi_colon, token_type::right_paren,
           token_type::colon, token_type::equal, token_type::not_equal, token_type::less_than,
           token_type::greater_than, token_type::less_equal_than, token_type::greater_equal_than},
          nullable_t{true}},
         {grammar_type::type, {token_type::id, token_type::right_brace, token_type::semi_colon}},
         {grammar_type::var_decl,
          {token_type::id, token_type::right_brace, token_type::id_func, token_type::id_integer,
           token_type::id_float, token_type::id_string, token_type::id_public,
           token_type::id_private}},
         {grammar_type::var_decl_rep, {token_type::id, token_type::right_brace}},
         {grammar_type::variable, {token_type::right_paren}},
         {grammar_type::variable_idnest, {token_type::right_paren}, nullable_t{true}},
         {grammar_type::variable_idnest_tail, {token_type::right_paren}, nullable_t{true}},
         {grammar_type::visibility,
          {token_type::id, token_type::id_func, token_type::id_integer, token_type::id_float,
           token_type::id_string},
          nullable_t{true}},
      }};
   } // namespace sets
} // namespace fr::grammar

/**
 * @brief A specialization for using the `grammar::rule` class in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<fr::grammar::rule>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const fr::grammar::rule& r, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{} -> {}", r.start(), r.tail());
   }
};
