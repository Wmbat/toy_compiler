#include <toy_compiler/munster/ast/decl/inheritance_decl.hpp>

namespace munster::ast
{
   inheritance_decl::inheritance_decl(const std::string& name,
                                      const front::source_location& location) :
      decl{name, location}
   {}

   auto inheritance_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "inheritance_decl", location().line,
                         location().column, lexeme());
   }
} // namespace munster::ast
