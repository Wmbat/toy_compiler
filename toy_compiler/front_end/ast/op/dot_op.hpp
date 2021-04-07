#pragma once

#include <toy_compiler/front_end/ast/op/op.hpp>

namespace front::ast
{
   class dot_op : public op
   {
   public:
      dot_op(node_ptr caller, node_ptr id, node_ptr callee);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
