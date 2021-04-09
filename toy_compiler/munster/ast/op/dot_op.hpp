#pragma once

#include <toy_compiler/munster/ast/op/op.hpp>

namespace munster::ast
{
   class dot_op : public op
   {
   public:
      dot_op(node_ptr caller, node_ptr id, node_ptr callee);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
