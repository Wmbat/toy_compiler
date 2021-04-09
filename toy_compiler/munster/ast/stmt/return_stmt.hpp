#pragma once

#include <toy_compiler/munster/ast/stmt/stmt.hpp>

namespace munster::ast
{
   class return_stmt : public stmt
   {
   public:
      return_stmt(node_ptr value, node_ptr expr);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
