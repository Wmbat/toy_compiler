#include <toy_compiler/munster/ast/decl/dot_decl.hpp>

namespace munster::ast
{
   dot_decl::dot_decl(const std::string& lexeme, const front::source_location& location) :
      decl{lexeme, location}
   {}

   auto dot_decl::to_string() const -> std::string { return "dot_decl"; }
} // namespace munster::ast
