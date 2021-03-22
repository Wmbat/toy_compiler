#include <toy_compiler/front_end/ast_bis/statement.hpp>

namespace front::ast
{
   stmt::stmt(const source_location& location) : node{location} {}
   stmt::stmt(const std::string& lexeme, const source_location& location) : node{lexeme, location}
   {}
} // namespace front::ast
