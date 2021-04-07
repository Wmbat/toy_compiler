#pragma once

#include <toy_compiler/front_end/ast/stmt/stmt.hpp>

namespace front::ast
{
   class read_stmt : public stmt
   {
   public:
      read_stmt(node_ptr loc, node_ptr compound_var_expr);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
