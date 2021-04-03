#include <toy_compiler/front_end/ast/node/float_expr.hpp>

namespace front::ast
{
   float_expr::float_expr(const std::string& lexeme, const source_location& location) :
      expr{lexeme, location}
   {}

   auto float_expr::to_string() const -> std::string
   {
      return fmt::format("float_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
