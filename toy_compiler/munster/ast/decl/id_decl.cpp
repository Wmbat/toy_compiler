#include <toy_compiler/munster/ast/decl/id_decl.hpp>

namespace munster::ast
{
   id_decl::id_decl(const std::string& lexeme, const source_location& location) :
      decl{lexeme, location}
   {}

   auto id_decl::to_string() const -> std::string { return fmt::format("{}", lexeme()); }
} // namespace munster::ast
