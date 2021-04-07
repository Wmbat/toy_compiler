#include <toy_compiler/front_end/ast_bis/factor.hpp>

#include <toy_compiler/front_end/ast/expr/var_expr.hpp>

#include <toy_compiler/front_end/ast/op/op.hpp>

namespace front::ast
{

   compound_parameter_expr_decl::compound_parameter_expr_decl(std::vector<node_ptr>&& params_decl)
   {
      make_family<expr, op>(std::move(params_decl));
   }

   compound_var_expr_decl::compound_var_expr_decl(std::vector<node_ptr>&& var_exprs)
   {
      make_family<variable_expr>(std::move(var_exprs));
   }

   auto compound_parameter_expr_decl::to_string() const -> std::string
   {
      return fmt::format("compound_parameter_expr_decl");
   }
   auto compound_var_expr_decl::to_string() const -> std::string
   {
      return fmt::format("compound_var_expr_decl");
   }
} // namespace front::ast
