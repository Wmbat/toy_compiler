#pragma once

#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class var_expr : public expr
   {
   public:
      var_expr(node_ptr id, node_ptr compound_array_indices);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
