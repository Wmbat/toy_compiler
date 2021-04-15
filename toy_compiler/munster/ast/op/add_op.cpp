#include <toy_compiler/munster/ast/op/add_op.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   add_op::add_op(node_ptr term_0, node_ptr value, node_ptr term_1) :
      op{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(term_0));
      make_child(std::move(term_1));
   }

   void add_op::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto add_op::to_string() const -> std::string
   {
      return fmt::format("operator{} {}", lexeme(), location());
   }
} // namespace munster::ast
