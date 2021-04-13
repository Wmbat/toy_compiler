#include <toy_compiler/munster/ast/decl/visibility_decl.hpp>

namespace munster::ast
{
   visibility_decl::visibility_decl(const std::string& name, const source_location& location) :
      decl{name, location}
   {}

   auto visibility_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "visibility_decl", location().line,
                         location().column, lexeme());
   }
} // namespace munster::ast
