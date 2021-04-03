#include <toy_compiler/front_end/ast_bis/factor.hpp>

#include <toy_compiler/front_end/ast/node/op.hpp>

namespace front::ast
{
   expr::expr(const source_location& location) : decl{location} {}
   expr::expr(const std::string& lexeme, const source_location& location) : decl{lexeme, location}
   {}

   variable_expr::variable_expr(node_ptr id, node_ptr compound_array_indices) :
      expr{std::string{id->lexeme()}, id->location()}
   {
      if (!std::empty(compound_array_indices->children()))
      {
         make_child(std::move(compound_array_indices));
      }
   }

   function_expr::function_expr(node_ptr id, node_ptr compound_input_parameter) :
      expr{std::string{id->lexeme()}, id->location()}
   {
      if (!std::empty(compound_input_parameter->children()))
      {
         make_child(std::move(compound_input_parameter));
      }
   }

   compound_parameter_expr_decl::compound_parameter_expr_decl(std::vector<node_ptr>&& params_decl)
   {
      make_family<expr, op>(std::move(params_decl));
   }

   compound_var_expr_decl::compound_var_expr_decl(std::vector<node_ptr>&& var_exprs)
   {
      make_family<variable_expr>(std::move(var_exprs));
   }

   ternary_expr::ternary_expr(node_ptr location, node_ptr condition, node_ptr expr_0,
                              node_ptr expr_1) :
      expr{location->location()}
   {
      make_child(std::move(condition));
      make_child(std::move(expr_0));
      make_child(std::move(expr_1));
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
   auto compound_var_expr_decl::to_string() const -> std::string
   {
      return fmt::format("compound_var_expr_decl");
   }
   auto ternary_expr::to_string() const -> std::string
   {
      return fmt::format("ternary_expr <line:{}, col:{}>", location().line, location().column);
   }
} // namespace front::ast
