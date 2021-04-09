#include <toy_compiler/munster/ast/expr/func_expr.hpp>

namespace munster::ast
{
   func_expr::func_expr(node_ptr id, node_ptr compound_input_parameter) :
      expr{std::string{id->lexeme()}, id->location()}
   {
      if (!std::empty(compound_input_parameter->children()))
      {
         make_child(std::move(compound_input_parameter));
      }
   }

   auto func_expr::to_string() const -> std::string
   {
      return fmt::format("function_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
