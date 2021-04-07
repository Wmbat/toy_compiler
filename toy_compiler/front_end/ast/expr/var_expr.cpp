#include <toy_compiler/front_end/ast/expr/var_expr.hpp>

namespace front::ast
{
   variable_expr::variable_expr(node_ptr id, node_ptr compound_array_indices) :
      expr{std::string{id->lexeme()}, id->location()}
   {
      if (!std::empty(compound_array_indices->children()))
      {
         make_child(std::move(compound_array_indices));
      }
   }

   auto variable_expr::to_string() const -> std::string
   {
      return fmt::format("variable_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
