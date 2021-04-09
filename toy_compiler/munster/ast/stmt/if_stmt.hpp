/**
 * @file if_stmt.hpp
 * @brief Contains the if_stmt node of the AST
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

#include <toy_compiler/munster/ast/stmt/stmt.hpp>

namespace munster::ast
{
   /**
    *
    */
   class if_stmt : public stmt
   {
   public:
      /**
       * @brief
       *
       * @param[in] location
       * @param[in] expr
       * @param[in] then_block
       * @param[in] else_block
       */
      if_stmt(node_ptr location, node_ptr expr, node_ptr then_block, node_ptr else_block);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
