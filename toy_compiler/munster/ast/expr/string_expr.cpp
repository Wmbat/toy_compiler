#include <toy_compiler/munster/ast/expr/string_expr.hpp>

namespace munster::ast
{
   string_expr::string_expr(const std::string& lexeme, const front::source_location& location) :
      expr{lexeme, location}
   {}

   auto string_expr::to_string() const -> std::string
   {
      return fmt::format("str_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
