#pragma once

#include <toy_compiler/munster/ast/stmt/stmt.hpp>

#include <toy_compiler/munster/ast/op/assign_op.hpp>

namespace munster::ast
{
   class assign_stmt : public stmt
   {
   public:
      assign_stmt(assign_op::ptr assign);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
