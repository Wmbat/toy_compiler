#include <toy_compiler/front_end/ast/node/priority_expr.hpp>

#include <toy_compiler/front_end/ast/node/op.hpp>

namespace front::ast
{
   priority_expr::priority_expr(node_ptr location, node_ptr expr_in) : expr{location->location()}
   {
      assert(dynamic_cast<location_decl*>(location.get())); // NOLINT
      // NOLINTNEXTLINE
      assert(dynamic_cast<expr*>(expr_in.get()) || dynamic_cast<op*>(expr_in.get()));

      make_child(std::move(expr_in));
   }

   auto priority_expr::to_string() const -> std::string
   {
      return fmt::format("expr_factor <line:{1}, col:{2}>", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
