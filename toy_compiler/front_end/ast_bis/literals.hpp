#pragma once

#include <toy_compiler/front_end/ast/node/node.hpp>

namespace front::ast
{
   class literal : public node
   {
   public:
      literal() = default;
      literal(const source_location& location);
      literal(const std::string& lexeme, const source_location& location);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class integer_literal : public literal
   {
   public:
      integer_literal(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
