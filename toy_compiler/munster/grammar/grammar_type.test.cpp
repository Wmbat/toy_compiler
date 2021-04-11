/**
 * @brief Runs tests on functions defined in grammar_type.hpp
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

#include <toy_compiler/munster/grammar/grammar_type.hpp>

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

TEST_SUITE("grammar/grammar_type.hpp test suite")
{
   TEST_CASE("fmt::formatter - grammar_type")
   {
      using namespace munster::grammar;

      namespace vi = ranges::views;

      for (auto i : magic_enum::enum_values<grammar_type>())
      {
         CHECK(fmt::format("{}", i) == magic_enum::enum_name(i));
      }
   }
}
