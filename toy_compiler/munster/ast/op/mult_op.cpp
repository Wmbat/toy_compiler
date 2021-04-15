#include <toy_compiler/munster/ast/op/mult_op.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   mult_op::mult_op(node_ptr factor_0, node_ptr value, node_ptr factor_1) :
      op{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(factor_0));
      make_child(std::move(factor_1));
   }

   void mult_op::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto mult_op::to_string() const -> std::string
   {
      return fmt::format("operator{} <line:{}, col:{}>", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
