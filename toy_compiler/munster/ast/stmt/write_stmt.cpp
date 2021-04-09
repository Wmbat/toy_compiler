#include <toy_compiler/munster/ast/stmt/write_stmt.hpp>

namespace munster::ast
{
   write_stmt::write_stmt(node_ptr value, node_ptr expr) :
      stmt{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(expr));
   }

   auto write_stmt::to_string() const -> std::string
   {
      return fmt::format("write_stmt <line:{}, col:{}>", location().line, location().column);
   }
} // namespace munster::ast
