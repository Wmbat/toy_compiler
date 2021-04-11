#include <toy_compiler/munster/ast/literal/int_literal.hpp>

namespace munster::ast
{
   integer_literal::integer_literal(const std::string& lexeme, const source_location& location) :
      literal{lexeme, location}
   {}

   auto integer_literal::to_string() const -> std::string
   {
      return fmt::format("integer_literal <line:{}, col:{}> {} '{}'", location().line,
                         location().column, "integer", lexeme());
   }
} // namespace munster::ast
