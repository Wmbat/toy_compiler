#pragma once

#include <toy_compiler/munster/ast/node.hpp>

namespace munster::ast
{
   class stmt : public node
   {
   public:
      stmt() = default;
      stmt(const front::source_location& location);
      stmt(const std::string& lexeme, const front::source_location& location);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };
} // namespace munster::ast