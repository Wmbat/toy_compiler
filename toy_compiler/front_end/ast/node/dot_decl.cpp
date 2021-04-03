#include <toy_compiler/front_end/ast/node/dot_decl.hpp>

namespace front::ast
{
   dot_decl::dot_decl(const std::string& lexeme, const source_location& location) :
      decl{lexeme, location}
   {}

   auto dot_decl::to_string() const -> std::string { return "dot_decl"; }
} // namespace front::ast
