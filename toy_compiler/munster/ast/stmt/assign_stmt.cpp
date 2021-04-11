#include <toy_compiler/munster/ast/stmt/assign_stmt.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   assign_stmt::assign_stmt(assign_op::ptr assign) { make_child(std::move(assign)); }

   void assign_stmt::accept(visitor_variant& visitor) const
   {
      using namespace mpark::patterns;

      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      const auto visit = [this](auto& vis) {
         vis(*this);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit);
   }

   auto assign_stmt::to_string() const -> std::string { return "assign_stmt"; }
} // namespace munster::ast
