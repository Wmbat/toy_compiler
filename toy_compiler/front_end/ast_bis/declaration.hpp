#pragma once

#include <toy_compiler/front_end/ast_bis/node.hpp>

namespace front::ast
{
   class decl : public node
   {
   public:
      decl() = default;
      decl(const source_location& location);
      decl(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class translation_unit_decl : public decl
   {
   public:
      translation_unit_decl(node_ptr compound_class, node_ptr compound_function, node_ptr main);

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

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class class_decl : public decl
   {
   public:
      class_decl(node_ptr class_start, node_ptr class_name, node_ptr compound_inheritance,
                 node_ptr compound_member);

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

   class member_decl : public decl
   {
   public:
      member_decl(node_ptr visibility, node_ptr var_or_func);

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

      [[nodiscard]] auto type() const -> std::string;

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      std::string m_type;
      std::string m_id;
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

   class member_function_decl : public decl
   {
   public:
      member_function_decl(node_ptr location, node_ptr id, node_ptr compound_params, node_ptr tail);

      [[nodiscard]] auto return_type() const -> std::string;
      [[nodiscard]] auto params() const -> std::vector<std::string>;

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      std::string m_return_type;
      std::vector<std::string> m_params;
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

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class main_decl : public decl
   {
   public:
      main_decl(node_ptr location, node_ptr func_body);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
