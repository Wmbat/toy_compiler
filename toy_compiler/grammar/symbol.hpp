/**
 * @file symbol.hpp
 * @brief Contains all element relating to the grammar::symbol class
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

      /**
       * List of string representation of the `grammar::symbol_type` enumeration values
       */
      constexpr std::array symbol_names{"terminal", "non_terminal", "stop"};
   } // namespace detail

   /**
    * @brief convert a `grammar::symbol_type` value to it's string representation
    *
    * @param [in] type The `grammar::symbol_type` value to represent as a string
    *
    * @return A `std::string_view` into the corresponding `grammar::symbol_type` string
    * representation.
    */
   constexpr auto to_string_view(grammar::symbol_type type) -> std::string_view
   {
      return detail::symbol_names.at(static_cast<std::uint32_t>(type));
   }

   /**
    * @brief A class used for holding an enum type value (`grammar::grammar_type` or
    * `grammar::token_type`) by using a agnostic layer using the `grammar::symbol_type` enum
    *
    * `grammar::token_type` values will be stored with the `grammar::symbol_type::terminal` enum
    * value & `grammar::grammar_type` values will be stored with the
    * `grammar::symbol_type::non_terminal` enum value. The `grammar::symbol_type::stop` is
    * placeholder used to designate the end of parsing
    */
   class symbol
   {
   public:
      using storage = std::variant<bool, token_type, grammar_type>;

      /**
       * @brief Construct a default **terminal** `grammar::symbol`
       */
      static constexpr auto terminal() -> symbol { return {token_type::max_size}; }
      /**
       * @brief Construct a default **non_terminal** `grammar::symbol`
       */
      static constexpr auto non_terminal() -> symbol { return {grammar_type::max_size}; }
      /**
       * @brief Construct a default **start** `grammar::symbol`
       */
      static constexpr auto start() -> symbol { return {grammar_type::start}; }
      /**
       * @brief Construct a default **stop** `grammar::symbol`
       */
      static constexpr auto stop() -> symbol { return {}; }

   public:
      /**
       * @brief Construct a `grammar::symbol` of type `grammar::symbol_type::terminal` from a
       * `grammar::token_type` value
       *
       * @param[in] value The `grammar::token_type` value to store
       */
      constexpr symbol(token_type value) : m_type{symbol_type::terminal}, m_data{value} {}
      /**
       * @brief Construct a `grammar::symbol` of type `grammar::symbol_type::non_terminal` from a
       * `grammar::grammar_type` value
       *
       * @param[in] value The `grammar::grammar_type` value to store
       */
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

      /**
       * @brief Get the type of the `grammar::symbol`
       */
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

   /**
    * @brief Check if a `grammar::symbol` is of type `symbol_type::stop`
    *
    * @param[in] s The input `grammar::symbol`
    *
    * @return True if the input `grammar::symbol` is of type `symbol_type::stop`
    */
   constexpr auto is_stop(const symbol& s) -> bool { return s.type() == symbol_type::stop; }
   /**
    * @brief Check if a `grammar::symbol` is of type `symbol_type::terminal`
    *
    * @param[in] s The input `grammar::symbol`
    *
    * @return True if the input `grammar::symbol` is of type `symbol_type::terminal`
    */
   constexpr auto is_terminal(const symbol& s) -> bool { return s.type() == symbol_type::terminal; }
   /**
    * @brief Check if a `grammar::symbol` is of type `symbol_type::non_terminal`
    *
    * @param[in] s The input `grammar::symbol`
    *
    * @return True if the input `grammar::symbol` is of type `symbol_type::non_terminal`
    */
   constexpr auto is_non_terminal(const symbol& s) -> bool
   {
      return s.type() == symbol_type::non_terminal;
   }

   /**
    * @brief Access the underlying `grammar::token_type` value stored in `grammar::symbol`.
    *
    * @param[in] s The `grammar::symbol` to access
    *
    * @throws std::bad_variant_access Thrown if the input `grammar::symbol` is not of type
    * `symbol_type::terminal`
    *
    * @return The `grammar::token_type` value stored in `grammar::symbol`
    */
   constexpr auto get_token_type(const symbol& s) -> token_type
   {
      return get<symbol_type::terminal>(s);
   }
   /**
    * @brief Access the underlying `grammar::grammar_type` value stored in `grammar::symbol`.
    *
    * @param[in] s The `grammar::symbol` to access
    *
    * @throws std::bad_variant_access Thrown if the input `grammar::symbol` is not of type
    * `symbol_type::non_terminal`
    *
    * @return The `grammar::grammar_type` value stored in `grammar::symbol`
    */
   constexpr auto get_grammar_type(const symbol& s) -> grammar_type
   {
      return get<symbol_type::non_terminal>(s);
   }
} // namespace grammar

/**
 * @brief A specialization for using the `grammar::grammar_type` enum in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<grammar::symbol_type>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(grammar::symbol_type type, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", grammar::to_string_view(type));
   }
};

/**
 * @brief A specialization for using the `grammar::symbol` class in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<grammar::symbol>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const grammar::symbol& s, FormatContext& ctx)
   {
      if (grammar::is_terminal(s))
      {
         return fmt::format_to(ctx.out(), "{}({})", s.type(), grammar::get_token_type(s));
      }

      if (grammar::is_non_terminal(s))
      {
         return fmt::format_to(ctx.out(), "{}({})", s.type(), grammar::get_grammar_type(s));
      }

      return fmt::format_to(ctx.out(), "{}", s.type());
   }
};
