#pragma once

#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class compound_var_expr_decl : public expr
   {
   public:
      compound_var_expr_decl(std::vector<node_ptr>&& var_exprs);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
