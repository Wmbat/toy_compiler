#pragma once

#include <toy_compiler/front_end/ast/decl/decl.hpp>

namespace front::ast
{
   class expr : public decl
   {
   public:
      expr() = default;
      expr(const source_location& location);
      expr(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };
} // namespace front::ast
