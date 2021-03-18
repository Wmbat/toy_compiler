#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class decl : public node
   {
   public:
      decl(const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;

   protected:
      [[nodiscard]] auto to_string_impl(sem::action_type type) const -> std::string;
   };

   class variable_decl final : public decl
   {
   public:
      variable_decl(node_ptr type, node_ptr item, node_ptr array_size_list,
                    const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class function_decl final : public decl
   {
   public:
      function_decl(node_ptr id, node_ptr function_param_list, node_ptr return_type_declaration,
                    const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class function_param_decl final : public decl
   {
   public:
      function_param_decl(node_ptr type, node_ptr id, node_ptr array_size_list,
                          const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class return_type_decl final : public decl
   {
   public:
      return_type_decl(node_ptr id, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class class_decl final : public decl
   {
   public:
      class_decl(node_ptr id, node_ptr inheritance_decl_list, node_ptr class_body_decl_list,
                 const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class class_body_decl final : public decl
   {
   public:
      class_body_decl(node_ptr visibility_decl, node_ptr member_decl,
                      const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class visibility_decl final : public decl
   {
   public:
      visibility_decl(node_ptr value, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class member_decl final : public decl
   {
   public:
      member_decl(node_ptr var_or_func_decl, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class inheritance_decl final : public decl
   {
   public:
      inheritance_decl(node_ptr id, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class idnest_decl : public decl
   {
   public:
      idnest_decl(node_ptr indice_list, node_ptr id, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class indice_decl : public decl
   {
   public:
      indice_decl(node_ptr expr, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace fr::ast
