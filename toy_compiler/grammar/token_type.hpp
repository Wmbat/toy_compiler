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

#include <array>
#include <cassert>
#include <cstdint>
#include <string_view>

namespace grammar
{
   namespace detail
   {
      /**
       * List of string representation of the `grammar::token_type` enumeration values
       */
      constexpr std::array token_names{"epsilon",      "id",
                                       "if",           "then",
                                       "else",         "integer",
                                       "float",        "string",
                                       "void",         "public",
                                       "private",      "func",
                                       "var",          "class",
                                       "while",        "read",
                                       "write",        "return",
                                       "main",         "inherits",
                                       "break",        "continue",
                                       "integer_lit",  "float_lit",
                                       "str_lit",      "period",
                                       "comma",        "colon",
                                       "double_colon", "semi_colon",
                                       "add",          "sub",
                                       "mult",         "div",
                                       "assign",       "or",
                                       "and",          "not",
                                       "qmark",        "equal",
                                       "less_than",    "less_equal_than",
                                       "greater_than", "greater_equal_than",
                                       "not_equal",    "left_brace",
                                       "right_brace",  "left_square",
                                       "right_square", "left_paren",
                                       "right_paren",  "line_cmt",
                                       "block_cmt",    "invalid_char",
                                       "invalid_id",   "invalid_num",
                                       "invalid_str",  "invalid_cmt"};

      /**
       * @brief All possible keywords of the language
       */
      constexpr std::array keywords{"if",     "then",  "else",     "integer", "float",
                                    "string", "void",  "public",   "private", "func",
                                    "var",    "class", "while",    "read",    "write",
                                    "return", "main",  "inherits", "break",   "continue"};
   } // namespace detail

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
      add_op,
      sub_op,
      mult_op,
      div_op,
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
      invalid_char,
      invalid_id,
      invalid_num,
      invalid_str,
      invalid_cmt,
      max_size
   };

   /**
    * @brief convert a `grammar::token_type` value to it's string representation
    *
    * @param [in] type The `grammar::token_type` value to represent as a string
    *
    * @return A `std::string_view` into the corresponding `grammar::token_type` string
    * representation from the `grammar::token_names` array
    */
   constexpr auto to_string_view(grammar::token_type type) -> std::string_view
   {
      return detail::token_names.at(static_cast<std::uint32_t>(type));
   }

   /**
    * @brief Find the enum representation of the keyword string
    *
    * @param [in] keyword_name The keyword string value
    *
    * @return The corresponding enum value to the keyword string
    */
   constexpr auto keyword_to_token_type(std::string_view keyword_name) -> token_type
   {
      if (keyword_name == detail::keywords[0])
      {
         return token_type::id_if;
      }
      else if (keyword_name == detail::keywords[1])
      {
         return token_type::id_then;
      }
      else if (keyword_name == detail::keywords[2])
      {
         return token_type::id_else;
      }
      else if (keyword_name == detail::keywords[3])
      {
         return token_type::id_integer;
      }
      else if (keyword_name == detail::keywords[4])
      {
         return token_type::id_float;
      }
      else if (keyword_name == detail::keywords[5]) // NOLINT
      {
         return token_type::id_string;
      }
      else if (keyword_name == detail::keywords[6]) // NOLINT
      {
         return token_type::id_void;
      }
      else if (keyword_name == detail::keywords[7]) // NOLINT
      {
         return token_type::id_public;
      }
      else if (keyword_name == detail::keywords[8]) // NOLINT
      {
         return token_type::id_private;
      }
      else if (keyword_name == detail::keywords[9]) // NOLINT
      {
         return token_type::id_func;
      }
      else if (keyword_name == detail::keywords[10]) // NOLINT
      {
         return token_type::id_var;
      }
      else if (keyword_name == detail::keywords[11]) // NOLINT
      {
         return token_type::id_class;
      }
      else if (keyword_name == detail::keywords[12]) // NOLINT
      {
         return token_type::id_while;
      }
      else if (keyword_name == detail::keywords[13]) // NOLINT
      {
         return token_type::id_read;
      }
      else if (keyword_name == detail::keywords[14]) // NOLINT
      {
         return token_type::id_write;
      }
      else if (keyword_name == detail::keywords[15]) // NOLINT
      {
         return token_type::id_return;
      }
      else if (keyword_name == detail::keywords[16]) // NOLINT
      {
         return token_type::id_main;
      }
      else if (keyword_name == detail::keywords[17]) // NOLINT
      {
         return token_type::id_inherits;
      }
      else if (keyword_name == detail::keywords[18]) // NOLINT
      {
         return token_type::id_break;
      }
      else if (keyword_name == detail::keywords[19]) // NOLINT
      {
         return token_type::id_continue;
      }
      else
      {
         assert("should never be reached"); // NOLINT

         return token_type::epsilon;
      }
   }

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
} // namespace grammar

/**
 * @brief A specialization for using the `grammar::token_type` enum in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<grammar::token_type>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(grammar::token_type t, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", to_string_view(t));
   }
};
