#pragma once

#include <toy_compiler/front_end/ast_bis/node.hpp>

namespace front::ast
{
   class op : public node
   {
   public:
      op() = default;
      op(const source_location& location);
      op(const std::string& lexeme, const source_location& location);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };
} // namespace front::ast
