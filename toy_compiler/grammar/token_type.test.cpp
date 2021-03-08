/**
 * @brief Runs tests on functions defined in token_type.hpp
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <toy_compiler/grammar/token_type.hpp>

#include <range/v3/view/iota.hpp>

template <typename T>
constexpr auto to(std::uint32_t i) -> T
{
   return static_cast<T>(i);
}

TEST_SUITE("grammar/token_type.hpp test suite")
{
   TEST_CASE("to_string_view()")
   {
      using namespace grammar;

      namespace vi = ranges::views;

      for (auto i : vi::iota(0u, static_cast<std::uint32_t>(token_type::max_size)))
      {
         CHECK(to_string_view(to<token_type>(i)) == detail::token_names[i]);
      }
   }
   TEST_CASE("keyword_to_token_type")
   {
      using namespace grammar;

      REQUIRE(keyword_to_token_type(detail::keywords[0]) == token_type::id_if);
      REQUIRE(keyword_to_token_type(detail::keywords[1]) == token_type::id_then);
      REQUIRE(keyword_to_token_type(detail::keywords[2]) == token_type::id_else);
      REQUIRE(keyword_to_token_type(detail::keywords[3]) == token_type::id_integer);
      REQUIRE(keyword_to_token_type(detail::keywords[4]) == token_type::id_float);
      REQUIRE(keyword_to_token_type(detail::keywords[5]) == token_type::id_string);
      REQUIRE(keyword_to_token_type(detail::keywords[6]) == token_type::id_void);
      REQUIRE(keyword_to_token_type(detail::keywords[7]) == token_type::id_public);
      REQUIRE(keyword_to_token_type(detail::keywords[8]) == token_type::id_private);
      REQUIRE(keyword_to_token_type(detail::keywords[9]) == token_type::id_func);
      REQUIRE(keyword_to_token_type(detail::keywords[10]) == token_type::id_var);
      REQUIRE(keyword_to_token_type(detail::keywords[11]) == token_type::id_class);
      REQUIRE(keyword_to_token_type(detail::keywords[12]) == token_type::id_while);
      REQUIRE(keyword_to_token_type(detail::keywords[13]) == token_type::id_read);
      REQUIRE(keyword_to_token_type(detail::keywords[14]) == token_type::id_write);
      REQUIRE(keyword_to_token_type(detail::keywords[15]) == token_type::id_return);
      REQUIRE(keyword_to_token_type(detail::keywords[16]) == token_type::id_main);
      REQUIRE(keyword_to_token_type(detail::keywords[17]) == token_type::id_inherits);
      REQUIRE(keyword_to_token_type(detail::keywords[18]) == token_type::id_break);
      REQUIRE(keyword_to_token_type(detail::keywords[19]) == token_type::id_continue);
   }
   TEST_CASE("is_token_invalid()")
   {
      using namespace grammar;

      namespace vi = ranges::views;

      for (auto i : vi::iota(0u, static_cast<std::uint32_t>(token_type::max_size)))
      {
         if (i >= 53 && i <= 57)
         {
            CHECK(is_token_invalid(to<token_type>(i)));
         }
         else
         {
            CHECK_FALSE(is_token_invalid(to<token_type>(i)));
         }
      }
   }
   TEST_CASE("fmt::formatter - token_type")
   {
      using namespace grammar;

      namespace vi = ranges::views;

      for (auto i : vi::iota(0u, static_cast<std::uint32_t>(token_type::max_size)))
      {
         CHECK(fmt::format("{}", to<token_type>(i)) == detail::token_names[i]);
      }
   }
}
