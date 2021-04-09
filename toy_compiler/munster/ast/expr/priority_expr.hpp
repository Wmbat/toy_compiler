#pragma once

#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class priority_expr : public expr
   {
   public:
      priority_expr(node_ptr location, node_ptr expr);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
