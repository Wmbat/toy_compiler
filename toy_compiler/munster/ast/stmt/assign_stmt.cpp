#include <toy_compiler/munster/ast/stmt/assign_stmt.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   assign_stmt::assign_stmt(assign_op::ptr assign) { make_child(std::move(assign)); }

   void assign_stmt::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto assign_stmt::to_string() const -> std::string { return "assign_stmt"; }
} // namespace munster::ast
