#include <toy_compiler/front_end/ast/node/not_expr.hpp>

#include <toy_compiler/front_end/ast/node/mult_op.hpp>

namespace front::ast
{
   not_expr::not_expr(node_ptr value, node_ptr factor_in) :
      expr{std::string{value->lexeme()}, value->location()}
   {
      assert(dynamic_cast<id_decl*>(value.get())); // NOLINT
      // NOLINTNEXTLINE
      assert(dynamic_cast<expr*>(factor_in.get()) || dynamic_cast<mult_op*>(factor_in.get()));

      make_child(std::move(factor_in));
   }

   auto not_expr::to_string() const -> std::string
   {
      return fmt::format("not_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
