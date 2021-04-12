/**
 * @file declaration.hpp
 * @brief Contains all data related to the AST declaration node
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

#include <toy_compiler/munster/ast/decl/decl.hpp>

#include <toy_compiler/munster/source_location.hpp>

namespace munster::ast
{
   class visibility_decl : public decl
   {
   public:
      visibility_decl(const std::string& name, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class compound_array_decl : public decl
   {
   public:
      compound_array_decl(std::vector<node_ptr>&& member_decl);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class array_decl : public decl
   {
   public:
      array_decl(node_ptr start_loc, node_ptr integer_lit, node_ptr end_loc);

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      source_location m_end_loc;
   };

   class compound_params_decl : public decl
   {
   public:
      compound_params_decl(std::vector<node_ptr>&& member_decl);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class compound_array_index_access_decl : public decl
   {
   public:
      /**
       * @brief Construct a **Compound Node** from a list of node_ptr.
       *
       * @param[in] variables The list of nodes that should belong to the compound declaration.
       *
       * @pre elements of variables must inherit from ast::op or ast::factor_decl.
       */
      compound_array_index_access_decl(std::vector<node_ptr>&& variables);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class array_index_access_decl : public decl
   {
   public:
      array_index_access_decl(node_ptr beg, node_ptr expr, node_ptr end);

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      source_location m_end;
   };

   class stmt_block_decl : public decl
   {
   public:
      using ptr = std::unique_ptr<stmt_block_decl>;

   public:
      stmt_block_decl(node_ptr node);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
