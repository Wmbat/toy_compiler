#pragma once

#include <toy_compiler/front_end/ast/op/op.hpp>

namespace front::ast
{
   class mult_op : public op
   {
   public:
      mult_op(node_ptr factor_0, node_ptr value, node_ptr factor_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
