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

#include <toy_compiler/munster/ast/node.hpp>
#include <toy_compiler/munster/ast/node_factory.hpp>
#include <toy_compiler/munster/lexer.hpp>
#include <toy_compiler/munster/parse_error.hpp>
#include <toy_compiler/util/logger.hpp>

#include <monads/maybe.hpp>

#include <span>
#include <unordered_map>

namespace munster
{
   /**
    * @brief
    */
   enum struct parse_status : std::uint32_t
   {
      success,
      error
   };

   /**
    * @brief
    */
   struct [[nodiscard]] parse_result
   {
      parse_status value = parse_status::success;
      ast::node_ptr ast = nullptr;
      std::string derivation;
      std::optional<std::vector<parse_error>> errors = {};
   };

   /**
    * @brief Parse an array of `front::lex_item` into a usable AST and performs simple syntax error
    * checking
    *
    * @param[in] items The lexed items to use for parsing
    */
   auto parse_items(std::span<const lex_item> items, util::logger_wrapper log = nullptr)
      -> parse_result;
} // namespace munster
