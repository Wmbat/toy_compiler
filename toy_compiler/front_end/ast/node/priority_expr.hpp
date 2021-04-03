#pragma once

#include <toy_compiler/front_end/ast_bis/factor.hpp>

namespace front::ast
{
   class priority_expr : public expr
   {
   public:
      priority_expr(node_ptr location, node_ptr expr);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
}
