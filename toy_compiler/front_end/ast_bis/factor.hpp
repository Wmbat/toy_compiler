#pragma once

#include <toy_compiler/front_end/ast/expr/expr.hpp>

namespace front::ast
{
   class compound_parameter_expr_decl : public decl
   {
   public:
      compound_parameter_expr_decl(std::vector<node_ptr>&& member_decl);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class compound_var_expr_decl : public expr
   {
   public:
      compound_var_expr_decl(std::vector<node_ptr>&& var_exprs);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
