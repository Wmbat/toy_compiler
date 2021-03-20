/**
 * @file symbol_table.hpp
 * @brief Contains all element relating to the `grammar::symbol_table` class
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

#include <toy_compiler/front_end/grammar/production.hpp>
#include <toy_compiler/front_end/grammar/symbol.hpp>

#include <array>

namespace fr::grammar
{
   /**
    * @brief A simple data structure to hold the table of rules needed for the table driven parsing.
    */
   class production_table
   {
      static constexpr std::size_t width = static_cast<std::size_t>(token_type::max_size);
      static constexpr std::size_t height =
         static_cast<std::size_t>(front::sem::grammar_type::max_size);

   public:
      /**
       * @brief The type used to access elements within the `symbol_table`
       */
      using key = std::pair<front::sem::grammar_type, token_type>;

   public:
      /**
       * @brief Access the `grammar::rule` associated with a certain `symbol_table::key`.
       *
       * @param[in] k The `symbol_table::key` to access the `grammar::rule`
       *
       * @return The `grammar::rule` associated with the `symbol_table::key` `k`
       */
      [[nodiscard]] auto lookup(const key& k) const -> const production&;
      /**
       * @brief Access the `grammar::rule` associated with a certain `symbol_table::key`.
       *
       * @param[in] k The `symbol_table::key` to access the `grammar::rule`
       *
       * @return The `grammar::rule` associated with the `symbol_table::key` `k`
       */
      auto lookup(const key& k) -> production&;

      /**
       * @brief Set a `grammar::rule` at a specific location in the table using a
       * `symbol_table::key`.
       *
       * @param[in] key The `symbol_table::key` to set the `grammar::rule`
       * @param[in] tail The tail values of the `grammar::rule`
       */
      void set_production(const key& key, symbol_array tail,
                          nullable_t nullable = nullable_t{false});

   private:
      std::array<std::array<grammar::production, height>, width> m_data;
   };

   auto construct_production_table() -> const grammar::production_table;
} // namespace fr::grammar
