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

#include <toy_compiler/munster/grammar/token_type.hpp>
#include <toy_compiler/munster/source_location.hpp>
#include <toy_compiler/util/logger.hpp>

#include <monads/maybe.hpp>

#include <filesystem>
#include <vector>

namespace munster
{
   /**
    * @brief Holds a the information of a parsed token.
    */
   struct lex_item
   {
      grammar::token_type type{}; // NOLINT
      std::string lexeme{};       // NOLINT
      source_location pos{};      // NOLINT

      auto operator<=>(const lex_item& other) const -> std::strong_ordering = default;
   };

   /**
    * @brief Opens a file and lexes it
    *
    * @param [in] path The path to the file
    * @param [in] log A logger used for status information
    *
    * @return The list of `fr::lex_item` from the file
    */
   auto lex_file(const std::filesystem::path& path, util::logger_wrapper log = nullptr)
      -> monad::maybe<std::vector<lex_item>>;
} // namespace munster

/**
 * @brief A specialization for using the `fr::lex_item` struct in the **fmt** & **spdlog** libraries
 */
template <>
struct fmt::formatter<munster::lex_item>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const munster::lex_item& tok, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "[.type = {0}, .lexeme = {1}, .position = {2}]", tok.type,
                            tok.lexeme, tok.pos);
   }
};
