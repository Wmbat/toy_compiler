#include <toy_compiler/front_end/ast/decl/type_decl.hpp>

namespace front::ast
{
   type_decl::type_decl(const std::string& name, const source_location& location) :
      decl{name, location}
   {}

   auto type_decl::to_string() const -> std::string { return fmt::format("{}", lexeme()); }
} // namespace front::ast
