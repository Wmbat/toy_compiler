#pragma once

#include <toy_compiler/munster/ast/stmt/stmt.hpp>

namespace munster::ast
{
   class while_stmt : public stmt
   {
   public:
      while_stmt(node_ptr loc, node_ptr expr, node_ptr stmt_block);

      void accept(visitor_variant &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
