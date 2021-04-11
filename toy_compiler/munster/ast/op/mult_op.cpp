#include <toy_compiler/munster/ast/op/mult_op.hpp>

#include <mpark/patterns.hpp>

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
      using namespace mpark::patterns;

      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      const auto visit = [this](auto& vis) {
         vis(*this);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit);
   }

   auto mult_op::to_string() const -> std::string
   {
      return fmt::format("operator{} <line:{}, col:{}>", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
