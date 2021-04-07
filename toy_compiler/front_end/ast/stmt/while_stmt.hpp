#pragma once

#include <toy_compiler/front_end/ast/stmt/stmt.hpp>

namespace front::ast
{
   class while_stmt : public stmt
   {
   public:
      while_stmt(node_ptr loc, node_ptr expr, node_ptr stmt_block);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
