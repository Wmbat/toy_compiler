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

#include <toy_compiler/front_end/ast_bis/node.hpp>

#include <toy_compiler/front_end/source_location.hpp>

namespace front::ast
{
   class decl : public node
   {
   public:
      decl() = default;
      decl(const source_location& location);
      decl(const std::string& lexeme, const source_location& location);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class translation_unit_decl : public decl
   {
   public:
      translation_unit_decl(node_ptr compound_class, node_ptr compound_function, node_ptr main);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class location_decl : public decl
   {
   public:
      location_decl(const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class type_decl : public decl
   {
   public:
      type_decl(const std::string& name, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class id_decl : public decl
   {
   public:
      id_decl(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class compound_class_decl : public decl
   {
   public:
      compound_class_decl(std::vector<node_ptr>&& class_decls);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class class_decl : public decl
   {
   public:
      class_decl(node_ptr class_start, node_ptr class_name, node_ptr compound_inheritance,
                 node_ptr compound_member);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class compound_inheritance_decl : public decl
   {
   public:
      compound_inheritance_decl(std::vector<node_ptr>&& class_decls);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class inheritance_decl : public decl
   {
   public:
      inheritance_decl(const std::string& name, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class compound_member_decl : public decl
   {
   public:
      compound_member_decl(std::vector<node_ptr>&& member_decl);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class visibility_decl : public decl
   {
   public:
      visibility_decl(const std::string& name, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class variable_decl : public decl
   {
   public:
      variable_decl(node_ptr type, node_ptr id, node_ptr compound_array);

      [[nodiscard]] auto type() const -> std::string_view;

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      std::string m_type;
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

   class compound_variable_decl : public decl
   {
   public:
      compound_variable_decl(std::vector<node_ptr>&& variables);

      void accept(visitor &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class main_decl : public decl
   {
   public:
      main_decl(node_ptr location, node_ptr func_body);

      void accept(visitor &visitor) const override;

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
      stmt_block_decl(node_ptr node);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
