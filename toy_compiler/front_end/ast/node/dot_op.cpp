#include <toy_compiler/front_end/ast/node/dot_op.hpp>

namespace front::ast
{
   dot_op::dot_op(node_ptr caller, node_ptr id, node_ptr callee) :
      op{std::string{id->lexeme()}, id->location()}
   {
      make_child(std::move(caller));
      make_child(std::move(callee));
   }

   auto dot_op::to_string() const -> std::string
   {
      return fmt::format("operator {} {}", lexeme(), location());
   }
} // namespace front::ast
