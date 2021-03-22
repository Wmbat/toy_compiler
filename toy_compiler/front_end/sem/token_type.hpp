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
      epsilon,
      id,
      id_if,
      id_then,
      id_else,
      id_integer,
      id_float,
      id_string,
      id_void,
      id_public,
      id_private,
      id_func,
      id_var,
      id_class,
      id_while,
      id_read,
      id_write,
      id_return,
      id_main,
      id_inherits,
      id_break,
      id_continue,
      integer_lit,
      float_lit,
      str_lit,
      period,
      comma,
      colon,
      double_colon,
      semi_colon,
      plus,
      minus,
      mult,
      div,
      assign,
      or_op,
      and_op,
      not_op,
      qmark,
      equal,
      less_than,
      less_equal_than,
      greater_than,
      greater_equal_than,
      not_equal,
      left_brace,
      right_brace,
      left_square,
      right_square,
      left_paren,
      right_paren,
      line_cmt,
      block_cmt,
      eof,
      invalid_char,
      invalid_id,
      invalid_num,
      invalid_str,
      invalid_cmt,
      max_size
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
      return (value == token_type::invalid_char) || (value == token_type::invalid_id) ||
         (value == token_type::invalid_num) || (value == token_type::invalid_cmt) ||
         (value == token_type::invalid_str);
   }

   constexpr auto is_eof(token_type value) -> bool { return token_type::eof == value; }

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
      return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(t));
   }
};
