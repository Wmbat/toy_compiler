/**
 * @file rule.hpp
 * @brief Contains all element relating to the `grammar::rule` class
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

#include <toy_compiler/grammar/grammar_type.hpp>
#include <toy_compiler/grammar/symbol.hpp>
#include <toy_compiler/grammar/token_type.hpp>

#include <libcaramel/containers/dynamic_array.hpp>

#include <fmt/ranges.h>

#include <memory>
#include <utility>

namespace grammar
{
   /**
    * @brief shorthand to designate a dynamic array containing `grammar::symbol`s with a small
    * buffer of 5 elements
    */
   using symbol_array = crl::small_dynamic_array<symbol, 5>; // NOLINT

   /**
    * @brief A simple class to hold the information of a grammar rule.
    *
    * The purpose of this class is to simplify the created of rules to use in the parsing table. The
    * rules are encoded using the following rule: **E -> aT`F**.
    */
   class rule
   {
   public:
      rule() = default;
      /**
       * @brief Create a rule
       *
       * @param[in] start The left hand `grammar::grammar_type` to use as the starting point of the
       * `grammar::rule`.
       * @param[in] tail The resulting `grammar::symbol`s derived from the `start`.
       */
      rule(grammar_type start, symbol_array tail);

      auto operator==(const rule& other) const -> bool = default;

      /**
       * @brief Access the start of the `grammar::rule`
       */
      [[nodiscard]] auto start() const -> grammar_type;

      /**
       * @brief Access the tail of the `grammar::rule`
       */
      [[nodiscard]] auto tail() const -> const symbol_array&;
      /**
       * @brief Access the tail of the `grammar::rule`
       */
      auto tail() -> symbol_array&;

   private:
      grammar_type m_start = grammar_type::max_size;
      symbol_array m_symbols{};
   };
} // namespace grammar

/**
 * @brief A specialization for using the `grammar::rule` class in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<grammar::rule>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const grammar::rule& r, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "({}, {})", r.start(), r.tail());
   }
};
