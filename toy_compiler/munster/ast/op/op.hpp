#pragma once

#include <toy_compiler/munster/ast/node.hpp>

namespace munster::ast
{
   class op : public node
   {
   public:
      op() = default;
      op(const source_location& location);
      op(const std::string& lexeme, const source_location& location);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };
} // namespace munster::ast
