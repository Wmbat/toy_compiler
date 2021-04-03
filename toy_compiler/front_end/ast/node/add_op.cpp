#include <toy_compiler/front_end/ast/node/add_op.hpp>

namespace front::ast
{
   add_op::add_op(node_ptr term_0, node_ptr value, node_ptr term_1) :
      op{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(term_0));
      make_child(std::move(term_1));
   }

   auto add_op::to_string() const -> std::string
   {
      return fmt::format("operator{} {}", lexeme(), location());
   }
} // namespace front::ast
