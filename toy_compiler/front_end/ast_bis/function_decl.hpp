#pragma once

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

namespace front::ast
{
   class compound_function_decl : public decl
   {
   public:
      compound_function_decl(std::vector<node_ptr>&& class_decls);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class function_decl : public decl
   {
   public:
      function_decl(node_ptr function_head, node_ptr statements);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class function_head_decl : public decl
   {
   public:
      function_head_decl(node_ptr location, node_ptr id, node_ptr class_method,
                         node_ptr function_param, node_ptr return_type);

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      std::string m_return_type;
      monad::maybe<std::string> m_class;
      monad::maybe<std::vector<std::string>> m_params;
   };

   class function_body_decl : public decl
   {
   public:
      function_body_decl(node_ptr variables, node_ptr statements);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
