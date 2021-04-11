#include <toy_compiler/munster/ast/decl/type_decl.hpp>

namespace munster::ast
{
   type_decl::type_decl(const std::string& name, const source_location& location) :
      decl{name, location}
   {}

   auto type_decl::to_string() const -> std::string { return fmt::format("{}", lexeme()); }
} // namespace munster::ast
