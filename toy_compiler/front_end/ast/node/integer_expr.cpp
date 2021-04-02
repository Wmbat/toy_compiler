#include <toy_compiler/front_end/ast/node/integer_expr.hpp>

namespace front::ast
{
   integer_expr::integer_expr(const std::string& lexeme, const source_location& location) :
      expr{lexeme, location}
   {}

   auto integer_expr::to_string() const -> std::string
   {
      return fmt::format("integer_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
