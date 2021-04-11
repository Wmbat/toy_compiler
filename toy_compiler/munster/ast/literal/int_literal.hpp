#pragma once

#include <toy_compiler/munster/ast/literal/literal.hpp>

namespace munster::ast
{
   class integer_literal : public literal
   {
   public:
      integer_literal(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
