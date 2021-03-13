/**
 * @file symbol_table.cpp
 * @brief Implement the functions defined at symbol_table.hpp
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

#include <toy_compiler/front_end/grammar/production_table.hpp>

#include <utility>

namespace fr::grammar
{
   auto production_table::lookup(const key& k) const -> const production&
   {
      const auto col = static_cast<std::size_t>(k.second);
      const auto row = static_cast<std::size_t>(k.first);

      return m_data[col][row]; // NOLINT
   }
   auto production_table::lookup(const key& k) -> production&
   {
      const auto col = static_cast<std::size_t>(k.second);
      const auto row = static_cast<std::size_t>(k.first);

      return m_data[col][row]; // NOLINT
   }

   void production_table::set_production(const key& key, symbol_array tail, nullable_t nullable)
   {
      const auto col = static_cast<std::size_t>(key.second);
      const auto row = static_cast<std::size_t>(key.first);

      m_data.at(col).at(row) = production{key.first, std::move(tail), nullable}; // NOLINT
   }
} // namespace fr::grammar
