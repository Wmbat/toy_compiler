#include <toy_compiler/munster/ast/expr/float_expr.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   float_expr::float_expr(const std::string& lexeme, const source_location& location) :
      expr{lexeme, location}
   {}

   void float_expr::accept(visitor_variant& visitor) const
   {
      visit_node(visitor, *this);
   }

   auto float_expr::to_string() const -> std::string
   {
      return fmt::format("float_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
