#pragma once

#include <toy_compiler/front_end/ast/node/node.hpp>

namespace front::ast
{
   class decl : public node
   {
   public:
      decl() = default;
      decl(const source_location& location);
      decl(const std::string& lexeme, const source_location& location);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };
} // namespace front::ast
