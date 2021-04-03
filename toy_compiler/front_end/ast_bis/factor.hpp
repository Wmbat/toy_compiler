#pragma once

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

namespace front::ast
{
   class expr : public decl
   {
   public:
      expr() = default;
      expr(const source_location& location);
      expr(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class variable_expr : public expr
   {
   public:
      variable_expr(node_ptr id, node_ptr compound_array_indices);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class function_expr : public expr
   {
   public:
      function_expr(node_ptr id, node_ptr compound_input_parameter);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class compound_parameter_expr_decl : public decl
   {
   public:
      compound_parameter_expr_decl(std::vector<node_ptr>&& member_decl);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class ternary_expr : public expr
   {
   public:
      ternary_expr(node_ptr location, node_ptr condition, node_ptr expr_0, node_ptr expr_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class compound_var_expr_decl : public expr
   {
   public:
      compound_var_expr_decl(std::vector<node_ptr>&& var_exprs);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
