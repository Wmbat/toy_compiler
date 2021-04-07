#include <toy_compiler/front_end/ast/decl/id_decl.hpp>

namespace front::ast
{
   id_decl::id_decl(const std::string& lexeme, const source_location& location) :
      decl{lexeme, location}
   {}

   auto id_decl::to_string() const -> std::string { return fmt::format("{}", lexeme()); }
} // namespace front::ast
