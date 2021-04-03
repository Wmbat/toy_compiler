#include <toy_compiler/front_end/ast/node/op.hpp>

namespace front::ast
{
   op::op(const source_location& location) : node{location} {}
   op::op(const std::string& lexeme, const source_location& location) : node{lexeme, location} {}

   void op::accept(visitor& /*visitor*/) const
   {
      assert(false && "Accept not implemented"); // NOLINT
   }

} // namespace front::ast
