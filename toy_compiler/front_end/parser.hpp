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

#include <toy_compiler/front_end/ast_bis/node.hpp>
#include <toy_compiler/front_end/lexer.hpp>
#include <toy_compiler/util/logger.hpp>

#include <monads/maybe.hpp>

#include <span>
#include <unordered_map>

namespace fr
{
   /**
    * @brief
    */
   enum struct parse_status : std::uint32_t
   {
      success,
      error
   };

   enum struct parse_error_type : std::uint32_t
   {
      syntax_error,
      max_size
   };

   struct parse_error
   {
      parse_error_type type{parse_error_type::max_size};
      front::source_location pos{};
      std::string lexeme;
      std::string line;
   };

   /**
    * @brief
    */
   struct [[nodiscard]] parse_result
   {
      parse_status value = parse_status::success;
      front::ast::node_ptr ast = nullptr;
      std::string derivation;
      monad::maybe<std::vector<parse_error>> errors = {};
   };

   /**
    * @brief Parse an array of `front::lex_item` into a usable AST and performs simple syntax error
    * checking
    *
    * @param[in] items The lexed items to use for parsing
    */
   auto parse_items(std::span<const front::lex_item> items, util::logger_wrapper log = nullptr)
      -> parse_result;
} // namespace fr

template <>
struct fmt::formatter<fr::parse_error_type>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(fr::parse_error_type error_type, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(error_type));
   }
};
