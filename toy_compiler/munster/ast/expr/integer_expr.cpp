#include <toy_compiler/munster/ast/expr/integer_expr.hpp>

#include <mpark/patterns.hpp>

using namespace mpark::patterns;

namespace munster::ast
{
   integer_expr::integer_expr(const std::string& lexeme, const front::source_location& location) :
      expr{lexeme, location}
   {}

   void integer_expr::accept(visitor_variant& visitor) const
   {
      const auto visit = [this](auto& vis) {
         vis(*this);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit);
   }

   auto integer_expr::to_string() const -> std::string
   {
      return fmt::format("integer_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
