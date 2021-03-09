/**
 * @brief Runs tests on functions defined in symbol_table.hpp
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

#include <toy_compiler/grammar/symbol_table.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/for_each.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>

#include <random>

using udist = std::uniform_int_distribution<std::uint32_t>;

template <typename T>
constexpr auto to(std::uint32_t i) -> T
{
   return static_cast<T>(i);
}

constexpr auto to_key(std::tuple<std::uint32_t, std::uint32_t> t) -> grammar::symbol_table::key
{
   return {to<grammar::grammar_type>(std::get<0>(t)), to<grammar::token_type>(std::get<1>(t))};
}

grammar::symbol_table create_table()
{
   using namespace grammar;

   namespace vi = ranges::views;

   const auto grammar_size = static_cast<std::uint32_t>(grammar_type::max_size);
   const auto token_size = static_cast<std::uint32_t>(token_type::max_size);

   std::mt19937 rng(std::random_device{}());
   std::uniform_int_distribution<std::uint32_t> grammar_size_dist{0, grammar_size};
   std::uniform_int_distribution<std::uint32_t> token_size_dist{0, grammar_size};

   symbol_table table;

   const auto all_keys =
      vi::cartesian_product(vi::iota(0u, grammar_size), vi::iota(0u, token_size)) |
      vi::transform(to_key);

   for (const auto k : all_keys)
   {
      symbol_array tail{};
      for (auto i : vi::iota(0u, 5u))
      {
         if (i % 2 == 0)
         {
            tail.append(to<grammar_type>(grammar_size_dist(rng)));
         }
         else
         {
            tail.append(to<token_type>(token_size_dist(rng)));
         }
      }

      table.set_rule(k, tail);
   }

   return table;
}

grammar::symbol_array generate_tail(std::mt19937& rng, udist& arr_size_dist,
                                    udist& grammar_size_dist, udist& token_size_dist)
{
   grammar::symbol_array tail{};
   for (auto i : ranges::views::iota(0u, arr_size_dist(rng)))
   {
      if (i % 2 == 0)
      {
         tail.append(to<grammar::grammar_type>(grammar_size_dist(rng)));
      }
      else
      {
         tail.append(to<grammar::token_type>(token_size_dist(rng)));
      }
   }
   return tail;
}

TEST_SUITE("grammar/symbol_table.hpp test suite")
{
   TEST_CASE("set_rule()")
   {
      using namespace grammar;
      symbol_table table;

      namespace vi = ranges::views;

      const auto grammar_size = static_cast<std::uint32_t>(grammar_type::max_size);
      const auto token_size = static_cast<std::uint32_t>(token_type::max_size);

      std::mt19937 rng(std::random_device{}());
      std::uniform_int_distribution<std::uint32_t> arr_size_dist{0, 10};
      std::uniform_int_distribution<std::uint32_t> grammar_size_dist{0, grammar_size};
      std::uniform_int_distribution<std::uint32_t> token_size_dist{0, token_size};

      const auto all_keys =
         vi::cartesian_product(vi::iota(0u, grammar_size), vi::iota(0u, token_size)) |
         vi::transform(to_key);

      for (const auto k : all_keys)
      {
         symbol_array tail = generate_tail(rng, arr_size_dist, grammar_size_dist, token_size_dist);

         table.set_rule(k, tail);

         CHECK(table.lookup(k).start() == k.first);
         CHECK(std::size(table.lookup(k).tail()) == std::size(tail));
      }
   }
   TEST_CASE("lookup()")
   {
      using namespace grammar;

      namespace vi = ranges::views;

      const auto grammar_size = static_cast<std::uint32_t>(grammar_type::max_size);
      const auto token_size = static_cast<std::uint32_t>(token_type::max_size);

      symbol_table table = create_table();

      const auto all_keys =
         vi::cartesian_product(vi::iota(0u, grammar_size), vi::iota(0u, token_size)) |
         vi::transform(to_key);

      for (const auto k : all_keys)
      {
         CHECK(table.lookup(k).start() == k.first);
         CHECK(std::size(table.lookup(k).tail()) == 5);
      }
   }
   TEST_CASE("lookup() const")
   {
      using namespace grammar;

      namespace vi = ranges::views;

      const auto grammar_size = static_cast<std::uint32_t>(grammar_type::max_size);
      const auto token_size = static_cast<std::uint32_t>(token_type::max_size);

      const symbol_table table = create_table();

      const auto all_keys =
         vi::cartesian_product(vi::iota(0u, grammar_size), vi::iota(0u, token_size)) |
         vi::transform(to_key);

      for (const auto k : all_keys)
      {
         CHECK(table.lookup(k).start() == k.first);
         CHECK(std::size(table.lookup(k).tail()) == 5);
      }
   }
}
