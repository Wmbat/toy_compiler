#pragma once

#include <toy_compiler/munster/ast/stmt/stmt.hpp>

namespace munster::ast
{
   class write_stmt : public stmt
   {
   public:
      write_stmt(node_ptr value, node_ptr expr);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
