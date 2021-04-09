#pragma once

#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class sign_expr : public expr
   {
   public:
      sign_expr(node_ptr sign, node_ptr factor);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
