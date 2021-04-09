#pragma once

#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class func_expr : public expr
   {
   public:
      func_expr(node_ptr id, node_ptr compound_input_parameter);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
