#include <toy_compiler/front_end/ast_bis/literals.hpp>

#include <range/v3/view/tail.hpp>

#include <cassert>

namespace front::ast_bis
{
   literal::literal(const fr::source_location& location) : node{location} {}
   literal::literal(const std::string& lexeme, const fr::source_location& location) :
      node{lexeme, location}
   {}

   integer_literal::integer_literal(const std::string& lexeme,
                                    const fr::source_location& location) :
      literal{lexeme, location}
   {}

   auto integer_literal::to_string() const -> std::string
   {
      return fmt::format("integer_literal <line:{}, col:{}> {} '{}'", location().line,
                         location().column, "integer", lexeme());
   }
} // namespace front::ast_bis
