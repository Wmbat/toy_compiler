#pragma once

#include <toy_compiler/munster/ast/op/op.hpp>

namespace munster::ast
{
   class assign_op : public op
   {
   public:
      using ptr = std::unique_ptr<assign_op>;

   public:
      assign_op(node_ptr val_0, node_ptr id_decl, node_ptr val_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
