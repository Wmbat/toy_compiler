#include <toy_compiler/front_end/ast_bis/statement.hpp>

namespace front::ast
{
   stmt::stmt(const source_location& location) : node{location} {}
   stmt::stmt(const std::string& lexeme, const source_location& location) : node{lexeme, location}
   {}

   write_stmt::write_stmt(node_ptr value, node_ptr expr) :
      stmt{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(expr));
   }

   return_stmt::return_stmt(node_ptr value, node_ptr expr) :
      stmt{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(expr));
   }

   break_stmt::break_stmt(const std::string& lexeme, const source_location& location) :
      stmt{lexeme, location}
   {}

   continue_stmt::continue_stmt(const std::string& lexeme, const source_location& location) :
      stmt{lexeme, location}
   {}

   auto write_stmt::to_string() const -> std::string
   {
      return fmt::format("write_stmt <line:{}, col:{}>", location().line, location().column);
   }
   auto return_stmt::to_string() const -> std::string
   {
      return fmt::format("return_stmt <line:{}, col:{}>", location().line, location().column);
   }
   auto break_stmt::to_string() const -> std::string
   {
      return fmt::format("break_stmt <line:{}, col:{}>", location().line, location().column);
   }
   auto continue_stmt::to_string() const -> std::string
   {
      return fmt::format("continue_stmt <line:{}, col:{}>", location().line, location().column);
   }
} // namespace front::ast
