#pragma once

#include <toy_compiler/munster/ast/node.hpp>

namespace munster::ast
{
   class literal : public node
   {
   public:
      literal() = default;
      literal(const front::source_location& location);
      literal(const std::string& lexeme, const front::source_location& location);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };
} // namespace munster::ast
