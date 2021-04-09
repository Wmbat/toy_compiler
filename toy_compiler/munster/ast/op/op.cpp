#include <toy_compiler/munster/ast/op/op.hpp>

namespace munster::ast
{
   op::op(const front::source_location& location) : node{location} {}
   op::op(const std::string& lexeme, const front::source_location& location) :
      node{lexeme, location}
   {}

   void op::accept(visitor_variant& /*visitor*/) const
   {
      assert(false && "Accept not implemented"); // NOLINT
   }
} // namespace munster::ast
