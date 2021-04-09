#pragma once

#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class float_expr : public expr
   {
   public:
      float_expr(const std::string& lexeme, const front::source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
