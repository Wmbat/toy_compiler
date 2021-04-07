#include <toy_compiler/front_end/ast/stmt/read_stmt.hpp>

namespace front::ast
{
   read_stmt::read_stmt(node_ptr loc, node_ptr var) : stmt{loc->location()}
   {
      make_child(std::move(var));
   }

   auto read_stmt::to_string() const -> std::string
   {
      return fmt::format("read_stmt <line:{}, col:{}>", location().line, location().column);
   }
} // namespace front::ast
