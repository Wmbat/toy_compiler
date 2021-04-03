#include <toy_compiler/front_end/ast/node/sign_expr.hpp>

namespace front::ast
{
   sign_expr::sign_expr(node_ptr sign, node_ptr factor_in) :
      expr{std::string{sign->lexeme()}, sign->location()}
   {
      make_child(std::move(factor_in));
   }

   auto sign_expr::to_string() const -> std::string
   {
      return fmt::format("sign_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
