/**
 * @file rule.cpp
 * @brief Implement the functions defined at rule.hpp
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

#include <toy_compiler/grammar/rule.hpp>
#include <utility>

namespace grammar
{
   rule::rule(grammar_type start, symbol_array tail) : m_start{start}, m_symbols{std::move(tail)} {}

   auto rule::start() const -> grammar_type { return m_start; }

   auto rule::tail() const -> const symbol_array& { return m_symbols; }
   auto rule::tail() -> symbol_array& { return m_symbols; }
} // namespace grammar
