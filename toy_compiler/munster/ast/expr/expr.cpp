#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   expr::expr(const front::source_location& location) : decl{location} {}
   expr::expr(const std::string& lexeme, const front::source_location& location) :
      decl{lexeme, location}
   {}
} // namespace munster::ast
