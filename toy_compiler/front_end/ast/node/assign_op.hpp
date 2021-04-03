#pragma once

#include <toy_compiler/front_end/ast/node/op.hpp>

namespace front::ast
{
   class assign_op : public op
   {
   public:
      assign_op(node_ptr val_0, node_ptr id_decl, node_ptr val_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
