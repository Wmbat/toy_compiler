#pragma once

#include <toy_compiler/munster/ast/node.hpp>

namespace munster::ast
{
   class decl : public node
   {
   public:
      decl() = default;
      decl(const source_location& location);
      decl(const std::string& lexeme, const source_location& location);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };
} // namespace munster::ast
