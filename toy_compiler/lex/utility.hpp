/**
 * @file utility.hpp
 * @brief Defines a set of utility functions used for the lexing of tokens.
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

#include <toy_compiler/lex/item.hpp>

#include <string_view>

#include <range/v3/algorithm/find.hpp>

namespace lex
{
   namespace grammar
   {
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

   namespace detail
   {
      constexpr std::array alphabet{
         std::to_array("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")};
      constexpr std::array digits{std::to_array("0123456789")};
      constexpr std::array operators{std::to_array("=+-*/<>|&!?")};
      constexpr std::array punctuations{grammar::period, grammar::comma, grammar::colon,
                                        grammar::semi_colon};
      constexpr std::array comments{"/*", "//"};
      constexpr std::array braces{grammar::open_parenth, grammar::close_parenth,
                                  grammar::open_square,  grammar::close_square,
                                  grammar::open_curly,   grammar::close_curly};
   } // namespace detail

   /**
    * @brief Check if a character is a letter from [a-zA-Z]
    */
   constexpr auto is_alphabet(char c) noexcept -> bool
   {
      return ranges::find(detail::alphabet, c) != std::end(detail::alphabet);
   }
   /**
    * @brief Check if a character is a number between [0, 9]
    */
   constexpr auto is_digit(char c) noexcept -> bool
   {
      return ranges::find(detail::digits, c) != std::end(detail::digits);
   }
   /**
    * @brief Check if a character is a letter from [a-zA-Z] or a number between [0, 9]
    */
   constexpr auto is_alphanum(char c) noexcept -> bool
   {
      return is_digit(c) || is_alphabet(c) || c == '_';
   }
   /**
    * @brief Check if a character is used for punctuation (.,:;)
    */
   constexpr auto is_punctuation(char c) noexcept -> bool
   {
      return ranges::find(detail::punctuations, c) != std::end(detail::punctuations);
   }
   /**
    * @brief Check if a character is an operator (=+-*<>/|&!?)
    */
   constexpr auto is_operator(char c) noexcept -> bool
   {
      return ranges::find(detail::operators, c) != std::end(detail::operators);
   }
   /**
    * @brief Check if a character is brace character ([](){})
    */
   constexpr auto is_braces(char c) noexcept -> bool
   {
      return ranges::find(detail::braces, c) != std::end(detail::braces);
   }
   /**
    * @brief Check if a character is a quotation (")
    */
   constexpr auto is_quote(char c) noexcept -> bool { return c == '\"'; }
   /**
    * @brief Check if a character is a newline character
    */
   constexpr auto is_newline(char c) noexcept -> bool { return c == '\n'; }
   /**
    * @brief Check if a character is a line or block comment character
    */
   constexpr auto is_comment(std::string_view str) -> bool
   {
      return ranges::find(detail::comments, str) != std::end(detail::comments);
   }

   /**
    * @brief Escapes all newline and cariage return characters in the string
    */
   auto to_literal(const std::string_view data) -> std::string;

   constexpr std::size_t kilobyte = 1024;
   /**
    * @brief Turn an unsigned integer literal into it's kilobyte representation.
    */
   constexpr auto operator""_kb(unsigned long long val) -> std::size_t { return val * kilobyte; }
} // namespace lex
