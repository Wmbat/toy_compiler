#include <toy_compiler/front_end/ast/node/string_expr.hpp>

namespace front::ast
{
   string_expr::string_expr(const std::string& lexeme, const source_location& location) :
      expr{lexeme, location}
   {}

   auto string_expr::to_string() const -> std::string
   {
      return fmt::format("str_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
