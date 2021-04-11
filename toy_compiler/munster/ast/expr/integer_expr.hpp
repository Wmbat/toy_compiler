#pragma once

#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class integer_expr : public expr
   {
   public:
      integer_expr(const std::string& lexeme, const front::source_location& location);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
