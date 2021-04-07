#include <toy_compiler/front_end/ast/expr/expr.hpp>

namespace front::ast
{
   expr::expr(const source_location& location) : decl{location} {}
   expr::expr(const std::string& lexeme, const source_location& location) : decl{lexeme, location}
   {
   }
} // namespace front::ast
