#pragma once

#include <toy_compiler/front_end/ast/expr/expr.hpp>

namespace front::ast
{
   class variable_expr : public expr
   {
   public:
      variable_expr(node_ptr id, node_ptr compound_array_indices);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
