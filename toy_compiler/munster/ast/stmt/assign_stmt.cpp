#include <toy_compiler/munster/ast/stmt/assign_stmt.hpp>

namespace munster::ast
{
   assign_stmt::assign_stmt(assign_op::ptr assign) { make_child(std::move(assign)); }

   auto assign_stmt::to_string() const -> std::string { return "assign_stmt"; }
} // namespace munster::ast

