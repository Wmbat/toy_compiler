#include <toy_compiler/munster/ast/expr/integer_expr.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   integer_expr::integer_expr(const std::string& lexeme, const source_location& location) :
      expr{lexeme, location}
   {}

   void integer_expr::accept(visitor_variant& visitor) const { visit_node(visitor, *this); }

   auto integer_expr::to_string() const -> std::string
   {
      return fmt::format("integer_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
