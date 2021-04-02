#include <toy_compiler/front_end/ast_bis/operator.hpp>

namespace front::ast
{
   op::op(const source_location& location) : node{location} {}
   op::op(const std::string& lexeme, const source_location& location) : node{lexeme, location} {}

   void op::accept(visitor& /*visitor*/) const
   {
      assert(false && "Accept not implemented"); // NOLINT
   }

   add_op::add_op(node_ptr term_0, node_ptr value, node_ptr term_1) :
      op{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(term_0));
      make_child(std::move(term_1));
   }

   mult_op::mult_op(node_ptr factor_0, node_ptr value, node_ptr factor_1) :
      op{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(factor_0));
      make_child(std::move(factor_1));
   }

   rel_op::rel_op(node_ptr arith_0, node_ptr value, node_ptr arith_1) :
      op{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(arith_0));
      make_child(std::move(arith_1));
   }

   auto add_op::to_string() const -> std::string
   {
      return fmt::format("operator {} <line:{}, col:{}>", lexeme(), location().line,
                         location().column);
   }
   auto mult_op::to_string() const -> std::string
   {
      return fmt::format("operator {} <line:{}, col:{}>", lexeme(), location().line,
                         location().column);
   }
   auto rel_op::to_string() const -> std::string
   {
      return fmt::format("operator {} <line:{}, col:{}>", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
