#include <toy_compiler/munster/ast/op/op.hpp>

namespace munster::ast
{
   op::op(const source_location& location) : node{location} {}
   op::op(const std::string& lexeme, const source_location& location) : node{lexeme, location} {}

   void op::accept(visitor_variant& /*visitor*/) const {}
} // namespace munster::ast
