#pragma once

#include <toy_compiler/front_end/ast/op/op.hpp>

namespace front::ast
{
   class add_op : public op
   {
   public:
      add_op(node_ptr term_0, node_ptr value, node_ptr term_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
