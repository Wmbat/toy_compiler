#pragma once

#include <toy_compiler/munster/ast/decl/id_decl.hpp>
#include <toy_compiler/munster/ast/expr/compound_param_expr_decl.hpp>
#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   class func_expr : public expr
   {
   public:
      func_expr(id_decl::ptr id, compound_parameter_expr_decl::ptr compound_input_parameter);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
