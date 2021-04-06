#include <toy_compiler/front_end/ast/node/func_or_assign_stmt.hpp>

#include <toy_compiler/front_end/ast/node/op.hpp>
#include <toy_compiler/front_end/ast_bis/factor.hpp>

namespace front::ast
{
   func_or_assign_stmt::func_or_assign_stmt(std::vector<node_ptr>&& var_or_assign_decls)
   {
      make_family<op, expr>(std::move(var_or_assign_decls));
   }

   void func_or_assign_stmt::accept(visitor& visitor) const { visitor.visit(this); }

   auto func_or_assign_stmt::to_string() const -> std::string { return "func_or_assign_stmt"; }
} // namespace front::ast
