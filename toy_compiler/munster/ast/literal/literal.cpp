#include <toy_compiler/munster/ast/literal/literal.hpp>

#include <range/v3/view/tail.hpp>

#include <cassert>

namespace munster::ast
{
   literal::literal(const source_location& location) : node{location} {}
   literal::literal(const std::string& lexeme, const source_location& location) :
      node{lexeme, location}
   {}

   void literal::accept(visitor_variant& /*visitor*/) const
   {
      assert(false && "Accept not implemented"); // NOLINT
   }
} // namespace munster::ast
