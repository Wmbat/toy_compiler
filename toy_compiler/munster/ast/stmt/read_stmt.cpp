#include <toy_compiler/munster/ast/stmt/read_stmt.hpp>

namespace munster::ast
{
   read_stmt::read_stmt(node_ptr loc, node_ptr var) : stmt{loc->location()}
   {
      make_child(std::move(var));
   }

   auto read_stmt::to_string() const -> std::string
   {
      return fmt::format("read_stmt <line:{}, col:{}>", location().line, location().column);
   }
} // namespace munster::ast
