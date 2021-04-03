/**
 * @file source_location.hpp
 * @brief Contains the elements related to the location in space of a front::lex_item in the source
 * file.
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

#include <compare>
#include <cstdint>
#include <limits>

namespace front
{
   /**
    * @brief Store the line and column data of a `lex_item`
    */
   struct source_location
   {
      std::uint32_t line = std::numeric_limits<std::uint32_t>::max();   // NOLINT
      std::uint32_t column = std::numeric_limits<std::uint32_t>::max(); // NOLINT

      /**
       * @brief Allow total order comparison between `source_location` objects
       *
       * @param[in] other The other `source_location` to compare with
       */
      constexpr auto operator<=>(const source_location& other) const
         -> std::strong_ordering = default;
   };
} // namespace front

template <>
struct fmt::formatter<front::source_location>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const front::source_location& pos, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "<line:{}, col:{}>", pos.line, pos.column);
   }
};
