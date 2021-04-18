#pragma once

#include <toy_compiler/munster/ast/expr/compound_param_expr_decl.hpp>

#include <toy_compiler/munster/ast/op/op.hpp>

namespace munster::ast
{
   compound_parameter_expr_decl::compound_parameter_expr_decl(std::vector<node_ptr>&& params_decl)
   {
      make_family<expr, op>(std::move(params_decl));
   }

   void compound_parameter_expr_decl::accept(visitor_variant& visitor) const
   {
      for (auto& child : children())
      {
         child->accept(visitor);
      }
   }

   auto compound_parameter_expr_decl::to_string() const -> std::string
   {
      return fmt::format("compound_parameter_expr_decl");
   }
} // namespace munster::ast
