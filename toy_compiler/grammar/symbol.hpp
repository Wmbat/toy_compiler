/**
 * @file symbol.hpp
 * @brief Contains all element relating to the `grammar::symbol` class
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
#include <toy_compiler/grammar/token_type.hpp>

#include <array>
#include <cstdint>
#include <string_view>
#include <variant>

namespace grammar
{
   /**
    * @brief All possible types of symbols that may be present during parsing.
    */
   enum struct symbol_type : std::uint32_t
   {
      terminal,
      non_terminal,
      stop
   };

   namespace detail
   {
      template <symbol_type T>
      struct convert_helper;

      template <>
      struct convert_helper<symbol_type::terminal>
      {
         using type = token_type;
      };

      template <>
      struct convert_helper<symbol_type::non_terminal>
      {
         using type = grammar_type;
      };

      template <>
      struct convert_helper<symbol_type::stop>
      {
         using type = bool;
      };
   } // namespace detail

   class symbol
   {
   public:
      using storage = std::variant<bool, token_type, grammar_type>;

      static constexpr auto terminal() -> symbol { return {token_type::max_size}; }
      static constexpr auto non_terminal() -> symbol { return {grammar_type::max_size}; }
      static constexpr auto start() -> symbol { return {grammar_type::start}; }
      static constexpr auto stop() -> symbol { return {}; }

   public:
      constexpr symbol(token_type value) : m_type{symbol_type::terminal}, m_data{value} {}
      constexpr symbol(grammar_type value) : m_type{symbol_type::non_terminal}, m_data{value} {}

      constexpr auto operator==(const symbol& symbol) const -> bool = default;
      constexpr auto operator==(token_type tok) const -> bool
      {
         if (m_type == symbol_type::terminal)
         {
            return std::get<token_type>(m_data) == tok;
         }

         return false;
      }
      constexpr auto operator==(grammar_type tok) const -> bool
      {
         if (m_type == symbol_type::non_terminal)
         {
            return std::get<grammar_type>(m_data) == tok;
         }

         return false;
      }

      [[nodiscard]] constexpr auto type() const -> symbol_type { return m_type; }

      template <symbol_type T>
      constexpr friend auto get(const symbol& s)
      {
         return std::get<typename detail::convert_helper<T>::type>(s.m_data);
      }

   private:
      constexpr symbol() = default;

   private:
      symbol_type m_type{symbol_type::stop};

      storage m_data{true};
   };

   constexpr auto is_stop(const symbol& s) -> bool { return s.type() == symbol_type::stop; }
   constexpr auto is_terminal(const symbol& s) -> bool { return s.type() == symbol_type::terminal; }
   constexpr auto is_non_terminal(const symbol& s) -> bool
   {
      return s.type() == symbol_type::non_terminal;
   }
} // namespace grammar
