#pragma once

#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class not_expr : public expr
   {
   public:
      not_expr(node_ptr value, node_ptr factor);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
