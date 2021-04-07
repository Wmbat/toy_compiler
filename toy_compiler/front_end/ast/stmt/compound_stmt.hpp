#pragma once

#include <toy_compiler/front_end/ast/stmt/stmt.hpp>

namespace front::ast
{
   class compound_stmt : public stmt
   {
   public:
      compound_stmt(std::vector<node_ptr>&& statements);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
