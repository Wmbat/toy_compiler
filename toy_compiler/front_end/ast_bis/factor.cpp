#include <toy_compiler/front_end/ast_bis/factor.hpp>

#include <toy_compiler/front_end/ast_bis/operator.hpp>

namespace front::ast
{
   expr::expr(const source_location& location) : decl{location} {}
   expr::expr(const std::string& lexeme, const source_location& location) : decl{lexeme, location}
   {}

   integer_expr::integer_expr(const std::string& lexeme, const source_location& location) :
      expr{lexeme, location}
   {}

   float_expr::float_expr(const std::string& lexeme, const source_location& location) :
      expr{lexeme, location}
   {}

   string_expr::string_expr(const std::string& lexeme, const source_location& location) :
      expr{lexeme, location}
   {}

   priority_expr::priority_expr(node_ptr location, node_ptr expr_in) : expr{location->location()}
   {
      assert(dynamic_cast<location_decl*>(location.get())); // NOLINT
      // NOLINTNEXTLINE
      assert(dynamic_cast<expr*>(expr_in.get()) || dynamic_cast<op*>(expr_in.get()));

      make_child(std::move(expr_in));
   }

   not_expr::not_expr(node_ptr value, node_ptr factor_in) :
      expr{std::string{value->lexeme()}, value->location()}
   {
      assert(dynamic_cast<id_decl*>(value.get())); // NOLINT
      // NOLINTNEXTLINE
      assert(dynamic_cast<expr*>(factor_in.get()) || dynamic_cast<mult_op*>(factor_in.get()));

      make_child(std::move(factor_in));
   }

   sign_expr::sign_expr(node_ptr sign, node_ptr factor_in) :
      expr{std::string{sign->lexeme()}, sign->location()}
   {
      make_child(std::move(factor_in));
   }

   func_or_var_expr::func_or_var_expr(std::vector<node_ptr>&& var_or_func_decls)
   {
      make_family<expr>(std::move(var_or_func_decls));
   }

   variable_expr::variable_expr(node_ptr id, node_ptr compound_array_indices) :
      expr{std::string{id->lexeme()}, id->location()}
   {
      if (compound_array_indices->child())
      {
         make_child(std::move(compound_array_indices));
      }
   }

   function_expr::function_expr(node_ptr id, node_ptr compound_input_parameter) :
      expr{std::string{id->lexeme()}, id->location()}
   {
      if (compound_input_parameter->child())
      {
         make_child(std::move(compound_input_parameter));
      }
   }

   compound_parameter_expr_decl::compound_parameter_expr_decl(std::vector<node_ptr>&& params_decl)
   {
      make_family<expr, op>(std::move(params_decl));
   }

   ternary_expr::ternary_expr(node_ptr location, node_ptr condition, node_ptr expr_0,
                              node_ptr expr_1) :
      expr{location->location()}
   {
      expr_0->make_sibling(std::move(expr_1));
      condition->make_sibling(std::move(expr_0));
      make_child(std::move(condition));
   }

   auto integer_expr::to_string() const -> std::string
   {
      return fmt::format("integer_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto float_expr::to_string() const -> std::string
   {
      return fmt::format("float_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto string_expr::to_string() const -> std::string
   {
      return fmt::format("str_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto priority_expr::to_string() const -> std::string
   {
      return fmt::format("expr_factor <line:{1}, col:{2}>", lexeme(), location().line,
                         location().column);
   }
   auto not_expr::to_string() const -> std::string
   {
      return fmt::format("not_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto sign_expr::to_string() const -> std::string
   {
      return fmt::format("sign_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto func_or_var_expr::to_string() const -> std::string
   {
      return fmt::format("func_or_var_expr");
   }
   auto variable_expr::to_string() const -> std::string
   {
      return fmt::format("variable_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto function_expr::to_string() const -> std::string
   {
      return fmt::format("function_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto compound_parameter_expr_decl::to_string() const -> std::string
   {
      return fmt::format("compound_parameter_expr_decl");
   }

   auto ternary_expr::to_string() const -> std::string
   {
      return fmt::format("ternary_expr <line:{}, col:{}>", location().line, location().column);
   }
} // namespace front::ast
