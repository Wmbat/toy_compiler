#include <toy_compiler/front_end/ast/expr.hpp>

namespace fr::ast
{
   static inline std::uint32_t counter = 0; // NOLINT;

   expr::expr(const source_location& location) : node{{}, location, counter++} {}
   expr::expr(node_ptr arith_expr, const source_location& location) : expr{location}
   {
      make_child(std::move(arith_expr));
   }
   expr::expr(node_ptr arith_expr_0, node_ptr rel_op, node_ptr arith_expr_1,
              const source_location& location) :
      expr{location}
   {
      rel_op->make_sibling(std::move(arith_expr_1));
      arith_expr_0->make_sibling(std::move(rel_op));
      make_child(std::move(arith_expr_0));
   }

   auto expr::to_string() const -> std::string { return to_string_impl(sem::action_type::expr); };

   auto expr::to_string_impl(sem::action_type type) const -> std::string
   {
      std::string name = fmt::format("\"{}_{}\"", magic_enum::enum_name(type), index());
      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, magic_enum::enum_name(type));

      return fmt::format("{};\n{}", name, output);
   }

   arith_expr::arith_expr(node_ptr term, node_ptr arith_tail_expr_list,
                          const source_location& location) :
      expr{location}
   {
      term->make_sibling(std::move(arith_tail_expr_list));
      make_child(std::move(term));
   }

   auto arith_expr::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::arith_expr);
   }

   arith_tail_expr::arith_tail_expr(node_ptr add_op, node_ptr term,
                                    const source_location& location) :
      expr{location}
   {
      add_op->make_sibling(std::move(term));
      make_child(std::move(add_op));
   }

   auto arith_tail_expr::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::arith_expr);
   }

   term_expr::term_expr(node_ptr factor, node_ptr term_tail_expr_list,
                        const source_location& location) :
      expr{location}
   {
      factor->make_sibling(std::move(term_tail_expr_list));
      make_child(std::move(factor));
   }

   auto term_expr::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::term_expr);
   }

   term_tail_expr::term_tail_expr(node_ptr mult_op, node_ptr factor,
                                  const source_location& location) :
      expr{location}
   {
      mult_op->make_sibling(std::move(factor));
      make_child(std::move(mult_op));
   }

   auto term_tail_expr::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::term_tail_expr);
   }

   variable_expr::variable_expr(node_ptr id, node_ptr idnest_list,
                                const source_location& location) :
      expr{location}
   {
      id->make_sibling(std::move(idnest_list));
      make_child(std::move(id));
   }

   factor_expr::factor_expr(const source_location& location) : expr{location} {}

   float_lit_factor_expr::float_lit_factor_expr(node_ptr value, const source_location& location) :
      expr{location}
   {
      make_child(std::move(value));
   };

   auto float_lit_factor_expr::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::factor_expr);
   }

   integer_lit_factor_expr::integer_lit_factor_expr(node_ptr value,
                                                    const source_location& location) :
      expr{location}
   {
      make_child(std::move(value));
   }

   auto integer_lit_factor_expr::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::factor_expr);
   }

   string_lit_factor_expr::string_lit_factor_expr(node_ptr value, const source_location& location) :
      expr{location}
   {
      make_child(std::move(value));
   }

   auto string_lit_factor_expr::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::factor_expr);
   }

   auto variable_expr::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::var_expr);
   }
} // namespace fr::ast
