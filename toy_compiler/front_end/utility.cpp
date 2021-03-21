/**
 * @file utility.cpp
 * @brief Implements the utility functions used for the lexing of tokens defined in utility.hpp.
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

#include <toy_compiler/front_end/utility.hpp>

#include <range/v3/algorithm/find.hpp>

namespace front
{
   auto to_literal(const std::string_view data) -> std::string
   {
      std::string res{data};

      {
         std::size_t start_pos = 0;
         while ((start_pos = res.find('\n', start_pos)) != std::string::npos)
         {
            res.replace(start_pos, 1, "\\n");
            start_pos += std::size("\\n");
         }
      }

      {
         std::size_t start_pos = 0;
         while ((start_pos = res.find('\r', start_pos)) != std::string::npos)
         {
            res.replace(start_pos, 1, "\\r");
            start_pos += std::size("\\r");
         }
      }

      return res;
   }
} // namespace front
