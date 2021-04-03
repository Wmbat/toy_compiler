#pragma once

#include <toy_compiler/front_end/ast_bis/factor.hpp>

namespace front::ast
{
   class sign_expr : public expr
   {
   public:
      sign_expr(node_ptr sign, node_ptr factor);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
