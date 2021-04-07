#pragma once

#include <toy_compiler/front_end/ast/op/op.hpp>

namespace front::ast
{
   class rel_op : public op
   {
   public:
      rel_op(node_ptr arith_expr_0, node_ptr value, node_ptr arith_expr_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
