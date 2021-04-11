#pragma once

#include <toy_compiler/munster/ast/op/op.hpp>

namespace munster::ast
{
   class rel_op : public op
   {
   public:
      rel_op(node_ptr arith_expr_0, node_ptr value, node_ptr arith_expr_1);

      void accept(visitor_variant &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
