#pragma once

#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class compound_parameter_expr_decl : public decl
   {
   public:
      using ptr = std::unique_ptr<compound_parameter_expr_decl>;

   public:
      compound_parameter_expr_decl(std::vector<node_ptr>&& member_decl);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
