#include <toy_compiler/munster/ast/stmt/continue_stmt.hpp>

namespace munster::ast
{
   continue_stmt::continue_stmt(const std::string& lexeme, const source_location& location) :
      stmt{lexeme, location}
   {}

   auto continue_stmt::to_string() const -> std::string
   {
      return fmt::format("continue_stmt {}", location());
   }
} // namespace munster::ast
