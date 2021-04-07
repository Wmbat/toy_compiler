#include <toy_compiler/front_end/ast/op/mult_op.hpp>

namespace front::ast
{
   mult_op::mult_op(node_ptr factor_0, node_ptr value, node_ptr factor_1) :
      op{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(factor_0));
      make_child(std::move(factor_1));
   }

   auto mult_op::to_string() const -> std::string
   {
      return fmt::format("operator{} <line:{}, col:{}>", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
