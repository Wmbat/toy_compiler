#pragma once

#include <toy_compiler/front_end/ast_bis/factor.hpp>

namespace front::ast
{
   class not_expr : public expr
   {
   public:
      not_expr(node_ptr value, node_ptr factor);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
