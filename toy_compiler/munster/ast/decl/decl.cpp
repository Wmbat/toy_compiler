#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   decl::decl(const source_location& location) : node{location} {}
   decl::decl(const std::string& lexeme, const source_location& location) : node{lexeme, location}
   {}

   void decl::accept(visitor_variant&) const
   {
      assert(false && "Accept not implemented"); // NOLINT
   }
} // namespace munster::ast
