#pragma once

#include <toy_compiler/front_end/ast_bis/factor.hpp>

namespace front::ast
{
   class float_expr : public expr
   {
   public:
      float_expr(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
