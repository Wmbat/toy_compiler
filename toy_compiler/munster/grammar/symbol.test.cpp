/**
 * @brief Runs tests on functions defined in symbol.hpp
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

#include "fmt/core.h"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <toy_compiler/munster/grammar/symbol.hpp>

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

template <typename T>
constexpr auto to(std::uint32_t i) -> T
{
   return static_cast<T>(i);
}

TEST_SUITE("grammar/symbol.hpp test suite")
{
   using namespace munster::grammar;

   namespace vi = ranges::views;

   TEST_CASE("static defaults")
   {
      constexpr auto terminal = symbol::terminal();
      constexpr auto non_terminal = symbol::non_terminal();
      constexpr auto start = symbol::start();
      constexpr auto stop = symbol::stop();

      CHECK(terminal.type() == symbol_type::terminal);
      CHECK(get<symbol_type::terminal>(terminal) == token_type::e_max_size);
      CHECK(non_terminal.type() == symbol_type::non_terminal);
      CHECK(get<symbol_type::non_terminal>(non_terminal) == grammar_type::max_size);
      CHECK(start.type() == symbol_type::non_terminal);
      CHECK(get<symbol_type::non_terminal>(start) == grammar_type::start);
      CHECK(stop.type() == symbol_type::stop);
      CHECK(get<symbol_type::stop>(stop) == true);
   }
   TEST_CASE("ctor")
   {
      SUBCASE("token_type")
      {
         const auto token_size = static_cast<std::uint32_t>(token_type::e_max_size);

         for (token_type type : vi::iota(0u, token_size) | vi::transform(to<token_type>))
         {
            symbol s{type};

            CHECK(s.type() == symbol_type::terminal);
            CHECK(get<symbol_type::terminal>(s) == type);
         }
      }
      SUBCASE("grammar_type")
      {
         const auto grammar_size = static_cast<std::uint32_t>(grammar_type::max_size);

         for (grammar_type type : vi::iota(0u, grammar_size) | vi::transform(to<grammar_type>))
         {
            symbol s{type};

            CHECK(s.type() == symbol_type::non_terminal);
            CHECK(get<symbol_type::non_terminal>(s) == type);
         }
      }
   }
   TEST_CASE("operator==")
   {
      SUBCASE("const symbol&")
      {
         symbol s1{grammar_type::add_op};
         symbol s1_prime{grammar_type::add_op};
         CHECK(s1 == s1_prime);

         symbol s2{token_type::e_max_size};
         CHECK(s2 == symbol::terminal());

         CHECK(s2 != s1);
      }
      SUBCASE("token_type")
      {
         using ranges::views::iota;

         for (std::uint32_t i : iota(0u, static_cast<std::uint32_t>(token_type::e_max_size)))
         {
            symbol s{static_cast<token_type>(i)};
            CHECK(s == static_cast<token_type>(i));
         }

         for (std::uint32_t i : iota(1u, static_cast<std::uint32_t>(token_type::e_max_size)))
         {
            symbol s{static_cast<token_type>(i)};
            CHECK_FALSE(s == static_cast<token_type>(i - 1));
         }
      }
      SUBCASE("grammar_type")
      {
         using ranges::views::iota;

         for (std::uint32_t i : iota(0u, static_cast<std::uint32_t>(grammar_type::max_size)))
         {
            symbol s{static_cast<grammar_type>(i)};
            CHECK(s == static_cast<grammar_type>(i));
         }

         for (std::uint32_t i : iota(1u, static_cast<std::uint32_t>(grammar_type::max_size)))
         {
            symbol s{static_cast<grammar_type>(i)};
            CHECK_FALSE(s == static_cast<grammar_type>(i - 1));
         }
      }
   }
   TEST_CASE("get()")
   {
      const auto token_size = static_cast<std::uint32_t>(token_type::e_max_size);
      const auto grammar_size = static_cast<std::uint32_t>(grammar_type::max_size);

      for (token_type type : vi::iota(0u, token_size) | vi::transform(to<token_type>))
      {
         symbol s{type};

         CHECK(s.type() == symbol_type::terminal);
         CHECK(get_token_type(s) == type);
         CHECK_THROWS(get<symbol_type::non_terminal>(s));
         CHECK_THROWS(get<symbol_type::stop>(s));
      }

      for (grammar_type type : vi::iota(0u, grammar_size) | vi::transform(to<grammar_type>))
      {
         symbol s{type};

         CHECK(s.type() == symbol_type::non_terminal);
         CHECK(get_grammar_type(s) == type);
         CHECK_THROWS(get<symbol_type::terminal>(s));
         CHECK_THROWS(get<symbol_type::stop>(s));
      }

      const auto stop = symbol::stop();
      CHECK(get<symbol_type::stop>(stop));
      CHECK_THROWS(get<symbol_type::non_terminal>(stop));
      CHECK_THROWS(get<symbol_type::terminal>(stop));
   }
   TEST_CASE("Utilities")
   {
      SUBCASE("is_terminal()")
      {
         using ranges::views::iota;

         for (std::uint32_t i : iota(0u, static_cast<std::uint32_t>(token_type::e_max_size)))
         {
            symbol s{static_cast<token_type>(i)};
            CHECK(is_terminal(s));
         }

         for (std::uint32_t i : iota(0u, static_cast<std::uint32_t>(grammar_type::max_size)))
         {
            symbol s{static_cast<grammar_type>(i)};
            CHECK_FALSE(is_terminal(s));
         }

         CHECK_FALSE(is_terminal(symbol::stop()));
         CHECK_FALSE(is_terminal(symbol::start()));
         CHECK_FALSE(is_terminal(symbol::non_terminal()));
         CHECK(is_terminal(symbol::terminal()));
      }
      SUBCASE("is_non_terminal()")
      {
         using ranges::views::iota;

         for (std::uint32_t i : iota(0u, static_cast<std::uint32_t>(token_type::e_max_size)))
         {
            symbol s{static_cast<token_type>(i)};
            CHECK_FALSE(is_non_terminal(s));
         }

         for (std::uint32_t i : iota(0u, static_cast<std::uint32_t>(grammar_type::max_size)))
         {
            symbol s{static_cast<grammar_type>(i)};
            CHECK(is_non_terminal(s));
         }

         CHECK_FALSE(is_non_terminal(symbol::stop()));
         CHECK(is_non_terminal(symbol::start()));
         CHECK(is_non_terminal(symbol::non_terminal()));
         CHECK_FALSE(is_non_terminal(symbol::terminal()));
      }
      SUBCASE("is_stop()")
      {
         using ranges::views::iota;

         for (std::uint32_t i : iota(0u, static_cast<std::uint32_t>(token_type::e_max_size)))
         {
            symbol s{static_cast<token_type>(i)};
            CHECK_FALSE(is_stop(s));
         }

         for (std::uint32_t i : iota(0u, static_cast<std::uint32_t>(grammar_type::max_size)))
         {
            symbol s{static_cast<grammar_type>(i)};
            CHECK_FALSE(is_stop(s));
         }

         CHECK(is_stop(symbol::stop()));
         CHECK_FALSE(is_stop(symbol::start()));
         CHECK_FALSE(is_stop(symbol::non_terminal()));
         CHECK_FALSE(is_stop(symbol::terminal()));
      }
   }

   TEST_CASE("to_string_view()")
   {
      using namespace munster::grammar;

      namespace vi = ranges::views;

      for (auto i : vi::iota(0u, static_cast<std::uint32_t>(symbol_type::stop) + 1))
      {
         CHECK(to_string_view(to<symbol_type>(i)) == munster::grammar::detail::symbol_names[i]);
      }
   }
   TEST_CASE("fmt::formatter - symbol_type")
   {
      using namespace munster::grammar;

      namespace vi = ranges::views;

      for (auto i : vi::iota(0u, static_cast<std::uint32_t>(symbol_type::stop) + 1))
      {
         CHECK(fmt::format("{}", to<symbol_type>(i)) == munster::grammar::detail::symbol_names[i]);
      }
   }
   TEST_CASE("fmt::formatter - symbol")
   {
      using namespace munster::grammar;

      namespace vi = ranges::views;

      const auto token_size = static_cast<std::uint32_t>(token_type::e_max_size);
      const auto grammar_size = static_cast<std::uint32_t>(grammar_type::max_size);

      for (token_type type : vi::iota(0u, token_size) | vi::transform(to<token_type>))
      {
         symbol s{type};

         std::string expected = fmt::format("{}({})", symbol_type::terminal, type);
         CHECK(fmt::format("{}", s) == expected);
      }

      for (grammar_type type : vi::iota(0u, grammar_size) | vi::transform(to<grammar_type>))
      {
         symbol s{type};

         std::string expected = fmt::format("{}({})", symbol_type::non_terminal, type);
         CHECK(fmt::format("{}", s) == expected);
      }

      CHECK(fmt::format("{}", symbol::stop()) == fmt::format("{}", symbol_type::stop));
   }
}
