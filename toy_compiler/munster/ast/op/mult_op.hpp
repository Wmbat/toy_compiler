#pragma once

#include <toy_compiler/munster/ast/op/op.hpp>

namespace munster::ast
{
   class mult_op : public op
   {
   public:
      mult_op(node_ptr factor_0, node_ptr value, node_ptr factor_1);

      void accept(visitor_variant &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
