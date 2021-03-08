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

#include <libcaramel/containers/dynamic_array.hpp>

#include <monads/maybe.hpp>

#include <span>
#include <unordered_map>

namespace parse
{
   /**
    * @brief
    */
   enum struct code : std::uint32_t
   {
      success,
      error
   };

   struct error
   {
   };

   /**
    * @brief
    */
   struct result
   {
      code value{code::success};
      monad::maybe<crl::dynamic_array<error>> errors;
   };

   /**
    * @param items The lexed items to use for parsing
    */
   auto parse_items(std::span<lex::item> items) -> result;
} // namespace parse
