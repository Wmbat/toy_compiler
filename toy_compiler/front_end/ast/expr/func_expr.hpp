#pragma once

#include <toy_compiler/front_end/ast/expr/expr.hpp>

namespace front::ast
{
   class function_expr : public expr
   {
   public:
      function_expr(node_ptr id, node_ptr compound_input_parameter);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
