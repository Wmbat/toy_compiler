#include <toy_compiler/munster/ast/op/dot_op.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   dot_op::dot_op(node_ptr caller, node_ptr id, node_ptr callee) :
      op{std::string{id->lexeme()}, id->location()}
   {
      make_child(std::move(caller));
      make_child(std::move(callee));
   }

   void dot_op::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto dot_op::to_string() const -> std::string
   {
      return fmt::format("operator{} {}", lexeme(), location());
   }
} // namespace munster::ast
