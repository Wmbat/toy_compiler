#include <toy_compiler/front_end/ast/operator.hpp>

namespace fr::ast
{
   static inline std::uint32_t counter = 0; // NOLINT;

   op::op(const source_location& location) : node{{}, location, counter++} {}

   qm_op::qm_op(node_ptr cond, node_ptr expr_1, node_ptr expr_2, const source_location& location) :
      op{location}
   {
      expr_1->make_sibling(std::move(expr_2));
      cond->make_sibling(std::move(expr_1));
      make_child(std::move(cond));
   }

   not_op::not_op(node_ptr factor_expr, const source_location& location) : op{location}
   {
      make_child(std::move(factor_expr));
   }

   mult_op::mult_op(node_ptr value, const source_location& location) : op{location}
   {
      make_child(std::move(value));
   }

   auto mult_op::to_string() const -> std::string { return fmt::format("{}", child()); }

   add_op::add_op(node_ptr value, const source_location& location) : op{location}
   {
      make_child(std::move(value));
   }

   auto add_op::to_string() const -> std::string { return fmt::format("{}", child()); }

   rel_op::rel_op(node_ptr value, const source_location& location) : op{location}
   {
      make_child(std::move(value));
   }

   auto rel_op::to_string() const -> std::string { return fmt::format("{}", child()); }
} // namespace fr::ast
