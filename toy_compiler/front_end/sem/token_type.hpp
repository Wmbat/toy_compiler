/**
 * @file token_type.hpp
 * @brief Contains all element relating to the `grammar::token_type` enum
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

#include <fmt/core.h>

#include <magic_enum.hpp>

#include <array>
#include <cassert>
#include <cstdint>
#include <string_view>

namespace front::sem
{
   /**
    * @brief All the possible item type that may be lexed from a source file
    */
   enum struct token_type : std::uint32_t
   {
      e_epsilon,
      e_id,
      e_if,
      e_then,
      e_else,
      e_integer,
      e_float,
      e_string,
      e_void,
      e_public,
      e_private,
      e_func,
      e_var,
      e_class,
      e_while,
      e_read,
      e_write,
      e_return,
      e_main,
      e_inherits,
      e_break,
      e_continue,
      e_integer_lit,
      e_float_lit,
      e_str_lit,
      e_dot,
      e_comma,
      e_colon,
      e_double_colon,
      e_semi_colon,
      e_plus,
      e_minus,
      e_mult,
      e_div,
      e_assign,
      e_or,
      e_and,
      e_not,
      e_qmark,
      e_equal,
      e_less_than,
      e_less_equal_than,
      e_greater_thane,
      e_greater_equal_than,
      e_not_equal,
      e_left_brace,
      e_right_brace,
      e_left_square,
      e_right_square,
      e_left_paren,
      e_right_paren,
      e_line_cmt,
      e_block_cmt,
      e_eof,
      e_invalid_char,
      e_invalid_id,
      e_invalid_num,
      e_invalid_str,
      e_invalid_cmt,
      e_max_size
   };

   /**
    * @brief Check if a `grammar::token_type` is invalid using the `invalid_` prefix in
    * `grammar::token_type`
    *
    * @param [in] value The 'grammar::token_type` value to check
    *
    * @return true if `value` represents an invalid `grammar::token_type`
    */
   constexpr auto is_token_invalid(token_type value) -> bool
   {
      return (value == token_type::e_invalid_char) || (value == token_type::e_invalid_id) ||
         (value == token_type::e_invalid_num) || (value == token_type::e_invalid_cmt) ||
         (value == token_type::e_invalid_str);
   }

   constexpr auto is_eof(token_type value) -> bool { return token_type::e_eof == value; }

   constexpr char period = '.';
   constexpr char comma = ',';
   constexpr char colon = ':';
   constexpr char semi_colon = ';';
   constexpr char open_parenth = '(';
   constexpr char close_parenth = ')';
   constexpr char open_square = '[';
   constexpr char close_square = ']';
   constexpr char open_curly = '{';
   constexpr char close_curly = '}';
} // namespace front::sem

/**
 * @brief A specialization for using the `sem::token_type` enum in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<front::sem::token_type>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(front::sem::token_type t, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(t).substr(2));
   }
};
