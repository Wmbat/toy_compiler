#include <toy_compiler/front_end/ast/node/rel_op.hpp>

namespace front::ast
{
   rel_op::rel_op(node_ptr arith_0, node_ptr value, node_ptr arith_1) :
      op{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(arith_0));
      make_child(std::move(arith_1));
   }

   auto rel_op::to_string() const -> std::string
   {
      return fmt::format("operator{} <line:{}, col:{}>", lexeme(), location().line,
                         location().column);
   }

} // namespace front::ast
