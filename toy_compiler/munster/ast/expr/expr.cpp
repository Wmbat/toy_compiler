#include <toy_compiler/munster/ast/expr/expr.hpp>

namespace munster::ast
{
   expr::expr(const source_location& location) : decl{location} {}
   expr::expr(const std::string& lexeme, const source_location& location) : decl{lexeme, location}
   {}

   void expr::accept(visitor_variant& /*visitor*/) const {}
} // namespace munster::ast
