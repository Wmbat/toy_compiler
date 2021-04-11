#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class expr : public decl
   {
   public:
      expr() = default;
      expr(const source_location& location);
      expr(const std::string& lexeme, const source_location& location);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };
} // namespace munster::ast
