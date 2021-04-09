#include <toy_compiler/munster/ast/expr/compound_var_expr_decl.hpp>

#include <toy_compiler/munster/ast/expr/var_expr.hpp>

namespace munster::ast
{
   compound_var_expr_decl::compound_var_expr_decl(std::vector<node_ptr>&& var_exprs)
   {
      make_family<var_expr>(std::move(var_exprs));
   }

   auto compound_var_expr_decl::to_string() const -> std::string
   {
      return fmt::format("compound_var_expr_decl");
   }
} // namespace munster::ast
