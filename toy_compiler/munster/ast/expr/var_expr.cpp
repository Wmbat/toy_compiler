#include <toy_compiler/munster/ast/expr/var_expr.hpp>

namespace munster::ast
{
   var_expr::var_expr(node_ptr id, node_ptr compound_array_indices) :
      expr{std::string{id->lexeme()}, id->location()}
   {
      if (!std::empty(compound_array_indices->children()))
      {
         make_child(std::move(compound_array_indices));
      }
   }

   auto var_expr::to_string() const -> std::string
   {
      return fmt::format("variable_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
