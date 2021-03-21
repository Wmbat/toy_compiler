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

#include <toy_compiler/front_end/sem/token_type.hpp>

#include <magic_enum.hpp>

#include <range/v3/view/iota.hpp>

template <typename T>
constexpr auto to(std::uint32_t i) -> T
{
   return static_cast<T>(i);
}

TEST_SUITE("grammar/token_type.hpp test suite")
{
   TEST_CASE("is_token_invalid()")
   {
      using namespace front::sem;

      namespace vi = ranges::views;

      for (auto i : vi::iota(0u, static_cast<std::uint32_t>(token_type::max_size)))
      {
         if (i >= 54 && i <= 58)
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
      using namespace front::sem;

      namespace vi = ranges::views;

      for (auto i : magic_enum::enum_values<token_type>())
      {
         CHECK(fmt::format("{}", i) == magic_enum::enum_name(i));
      }
   }
}
