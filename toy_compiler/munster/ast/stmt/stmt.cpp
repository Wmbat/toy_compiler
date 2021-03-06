#include <toy_compiler/munster/ast/stmt/stmt.hpp>

namespace munster::ast
{
   stmt::stmt(const source_location& location) : node{location} {}
   stmt::stmt(const std::string& lexeme, const source_location& location) : node{lexeme, location}
   {}

   void stmt::accept(visitor_variant& /*visitor*/) const {}
} // namespace munster::ast
