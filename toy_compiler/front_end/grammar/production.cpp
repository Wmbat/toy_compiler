/**
 * @file production.cpp
 * @brief Implement the functions defined at production.hpp
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

#include <toy_compiler/front_end/grammar/production.hpp>
#include <utility>

namespace fr::grammar
{
   production::production(grammar_type start, symbol_array tail, nullable_t nullable) :
      m_start{start},
      m_symbols{std::move(tail)},
      m_is_nullable{nullable}
   {}

   auto production::start() const -> grammar_type { return m_start; }

   auto production::tail() const -> const symbol_array& { return m_symbols; }
   auto production::tail() -> symbol_array& { return m_symbols; }

   auto production::nullable() const noexcept -> bool { return m_is_nullable.value(); }
} // namespace fr::grammar
