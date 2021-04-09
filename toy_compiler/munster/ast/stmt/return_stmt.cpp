#include <toy_compiler/munster/ast/stmt/return_stmt.hpp>

namespace munster::ast
{
   return_stmt::return_stmt(node_ptr value, node_ptr expr) :
      stmt{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(expr));
   }

   auto return_stmt::to_string() const -> std::string
   {
      return fmt::format("return_stmt <line:{}, col:{}>", location().line, location().column);
   }
} // namespace munster::ast
