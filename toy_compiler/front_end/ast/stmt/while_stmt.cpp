#include <toy_compiler/front_end/ast/stmt/while_stmt.hpp>

#include <toy_compiler/front_end/ast/decl/location_decl.hpp>
#include <toy_compiler/front_end/ast_bis/declaration.hpp>

namespace front::ast
{
   while_stmt::while_stmt(node_ptr loc, node_ptr expr, node_ptr stmt_block) : stmt{loc->location()}
   {
      assert(detail::is_type<location_decl>(loc)); // NOLINT

      make_child(std::move(expr));
      make_child(std::move(stmt_block));
   }

   auto while_stmt::to_string() const -> std::string
   {
      return fmt::format("while_stmt <line:{}, col:{}>", location().line, location().column);
   }
} // namespace front::ast
