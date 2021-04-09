#include <toy_compiler/munster/ast/stmt/break_stmt.hpp>

namespace munster::ast
{
   break_stmt::break_stmt(const std::string& lexeme, const front::source_location& location) :
      stmt{lexeme, location}
   {}

   auto break_stmt::to_string() const -> std::string
   {
      return fmt::format("break_stmt <line:{}, col:{}>", location().line, location().column);
   }
} // namespace munster::ast
