#include <toy_compiler/front_end/ast/decl/decl.hpp>

namespace front::ast
{
   decl::decl(const source_location& location) : node{location} {}
   decl::decl(const std::string& lexeme, const source_location& location) : node{lexeme, location}
   {}

   void decl::accept(visitor&) const
   {
      assert(false && "Accept not implemented"); // NOLINT
   }
} // namespace front::ast
