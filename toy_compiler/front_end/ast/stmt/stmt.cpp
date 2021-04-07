#include <toy_compiler/front_end/ast/stmt/stmt.hpp>

namespace front::ast
{
   stmt::stmt(const source_location& location) : node{location} {}
   stmt::stmt(const std::string& lexeme, const source_location& location) : node{lexeme, location}
   {}

   void stmt::accept(visitor& /*visitor*/) const {}
} // namespace front::ast
