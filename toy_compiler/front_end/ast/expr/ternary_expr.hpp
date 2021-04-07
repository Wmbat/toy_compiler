#pragma once

#include <toy_compiler/front_end/ast/expr/expr.hpp>

namespace front::ast
{
   class ternary_expr : public expr
   {
   public:
      ternary_expr(node_ptr location, node_ptr condition, node_ptr expr_0, node_ptr expr_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
