#include <toy_compiler/munster/ast/op/rel_op.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   rel_op::rel_op(node_ptr arith_0, node_ptr value, node_ptr arith_1) :
      op{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(arith_0));
      make_child(std::move(arith_1));
   }

   void rel_op::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto rel_op::to_string() const -> std::string
   {
      return fmt::format("operator{} <line:{}, col:{}>", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
