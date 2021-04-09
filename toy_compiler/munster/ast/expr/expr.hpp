#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class expr : public decl
   {
   public:
      expr() = default;
      expr(const front::source_location& location);
      expr(const std::string& lexeme, const front::source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };
} // namespace munster::ast
