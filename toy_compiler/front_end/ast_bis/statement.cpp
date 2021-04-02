#include <toy_compiler/front_end/ast_bis/statement.hpp>

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <utility>

namespace front::ast
{
   stmt::stmt(const source_location& location) : node{location} {}
   stmt::stmt(const std::string& lexeme, const source_location& location) : node{lexeme, location}
   {}

   void stmt::accept(visitor& /*visitor*/) const
   {
      assert(false && "Accept not implementd"); // NOLINT
   }

   if_stmt::if_stmt(node_ptr loc, node_ptr expr, node_ptr then_block, node_ptr else_block) :
      stmt{loc->location()}
   {
      assert(detail::is_type<location_decl>(loc));          // NOLINT
      assert(detail::is_type<stmt_block_decl>(then_block)); // NOLINT
      assert(detail::is_type<stmt_block_decl>(else_block)); // NOLINT

      make_child(std::move(expr));
      make_child(std::move(then_block));
      make_child(std::move(else_block));
   }

   while_stmt::while_stmt(node_ptr loc, node_ptr expr, node_ptr stmt_block) : stmt{loc->location()}
   {
      assert(detail::is_type<location_decl>(loc)); // NOLINT

      make_child(std::move(expr));
      make_child(std::move(stmt_block));
   }

   write_stmt::write_stmt(node_ptr value, node_ptr expr) :
      stmt{std::string{value->lexeme()}, value->location()}
   {
      make_child(std::move(expr));
   }

   read_stmt::read_stmt(node_ptr loc, node_ptr var)
   {
      assert(detail::is_type<location_decl>(loc)); // NOLINT

      make_child(std::move(var));
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

   auto if_stmt::to_string() const -> std::string
   {
      return fmt::format("if_stmt <line:{}, col:{}>", location().line, location().column);
   }
   auto while_stmt::to_string() const -> std::string
   {
      return fmt::format("while_stmt <line:{}, col:{}>", location().line, location().column);
   }
   auto read_stmt::to_string() const -> std::string
   {
      return fmt::format("read_stmt <line:{}, col:{}>", location().line, location().column);
   }
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
