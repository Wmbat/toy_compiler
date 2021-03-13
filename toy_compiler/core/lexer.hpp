/**
 * @file lexer.hpp
 * @brief Contains all the lexing functions for the compiler
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

#include <toy_compiler/grammar/token_type.hpp>
#include <toy_compiler/util/logger.hpp>

#include <monads/maybe.hpp>

#include <filesystem>
#include <vector>

namespace lex
{
   /**
    * @brief Holds a the information of a parsed token.
    */
   struct item
   {
      grammar::token_type type{}; // NOLINT
      std::string lexeme{};       // NOLINT
      std::uint32_t line{};       // NOLINT
      std::uint32_t column{};     // NOLINT

      auto operator<=>(const item& other) const -> std::strong_ordering = default;
   };

   /**
    * @brief Opens a file and lexes it
    *
    * @param [in] path The path to the file
    * @param [in] log A logger used for status information
    *
    * @return The list of `lex::item` from the file
    */
   auto lex_file(const std::filesystem::path& path, util::logger_wrapper log = nullptr)
      -> monad::maybe<std::vector<lex::item>>;
} // namespace lex

/**
 * @brief A specialization for using the `lex::item` struct in the **fmt** & **spdlog** libraries
 */
template <>
struct fmt::formatter<lex::item>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const lex::item& tok, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "[.type = {0}, .lexeme = {1}, .line = {2}]",
                            tok.type, tok.lexeme, tok.line);
   }
};
