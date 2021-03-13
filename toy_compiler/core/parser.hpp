/**
 * @file parser.hpp
 * @brief Contains all the parsing functions for the compiler
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

#include <toy_compiler/core/lexer.hpp>
#include <toy_compiler/grammar/symbol.hpp>
#include <toy_compiler/util/logger.hpp>

#include <monads/maybe.hpp>

#include <span>
#include <unordered_map>

namespace parse
{
   /**
    * @brief
    */
   enum struct status : std::uint32_t
   {
      success,
      error
   };

   enum struct error_type : std::uint32_t
   {
      missing_terminal,
      max_size
   };

   struct error
   {
      error_type type{error_type::max_size};
      grammar::token_type token;
      std::uint32_t line_number;
      std::string line;
   };

   /**
    * @brief
    */
   struct [[nodiscard]] result
   {
      status value{status::success};
      monad::maybe<std::vector<error>> errors;
   };

   /**
    * @param items The lexed items to use for parsing
    */
   auto parse_items(std::span<const lex::item> items, util::logger_wrapper log = nullptr) -> result;
} // namespace parse
