/**
 * @brief Runs tests on functions defined in production.hpp
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

#include <toy_compiler/front_end/grammar/production.hpp>

#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

#include <random>

template <typename T>
constexpr auto to(std::uint32_t i) -> T
{
   return static_cast<T>(i);
}

TEST_SUITE("grammar/production.hpp test suite")
{
   TEST_CASE("empty production")
   {
      using namespace fr::grammar;

      production r1{};

      CHECK(r1.start() == grammar_type::max_size);
      CHECK(std::empty(r1.tail()));
   }
   TEST_CASE("normal production")
   {
      using namespace fr::grammar;

      namespace vi = ranges::views;

      const auto grammar_size = static_cast<std::uint32_t>(grammar_type::max_size);

      std::mt19937 rng(std::random_device{}());
      std::uniform_int_distribution<std::uint32_t> arr_size_dist{0, 10};
      std::uniform_int_distribution<std::uint32_t> grammar_size_dist{0, grammar_size};
      std::uniform_int_distribution<std::uint32_t> token_size_dist{0, grammar_size};

      for (auto type : vi::iota(0u, grammar_size) | vi::transform(to<grammar_type>))
      {
         symbol_array tail{};
         for (auto i : vi::iota(0u, arr_size_dist(rng)))
         {
            if (i % 2 == 0)
            {
               tail.push_back(to<grammar_type>(grammar_size_dist(rng)));
            }
            else
            {
               tail.push_back(to<token_type>(token_size_dist(rng)));
            }
         }

         production r{type, tail};

         CHECK(r.start() == type);
         CHECK(r.tail() == tail);
      }
   }
   TEST_CASE("fmt::formatter - production")
   {
      using namespace fr::grammar;

      namespace vi = ranges::views;

      const auto grammar_size = static_cast<std::uint32_t>(grammar_type::max_size);

      std::mt19937 rng(std::random_device{}());
      std::uniform_int_distribution<std::uint32_t> arr_size_dist{0, 10};
      std::uniform_int_distribution<std::uint32_t> grammar_size_dist{0, grammar_size};
      std::uniform_int_distribution<std::uint32_t> token_size_dist{0, grammar_size};

      for (auto type : vi::iota(0u, grammar_size) | vi::transform(to<grammar_type>))
      {
         symbol_array tail{};
         for (auto i : vi::iota(0u, arr_size_dist(rng)))
         {
            if (i % 2 == 0)
            {
               tail.push_back(to<grammar_type>(grammar_size_dist(rng)));
            }
            else
            {
               tail.push_back(to<token_type>(token_size_dist(rng)));
            }
         }

         production r{type, tail};

         std::string expected = fmt::format("{} -> {}", type, tail);
         CHECK(fmt::format("{}", r) == expected);
      }
   }
}
