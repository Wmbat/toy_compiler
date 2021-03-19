#pragma once

#include <toy_compiler/front_end/ast_bis/node.hpp>

namespace front::ast_bis
{
   class literal : public node
   {
   public:
      literal() = default;
      literal(const fr::source_location& location);
      literal(const std::string& lexeme, const fr::source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class integer_literal : public literal
   {
   public:
      integer_literal(const std::string& lexeme, const fr::source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast_bis
