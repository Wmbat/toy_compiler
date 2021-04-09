#include <toy_compiler/munster/ast/stmt/func_stmt.hpp>

#include <toy_compiler/munster/ast/expr/expr.hpp>
#include <toy_compiler/munster/ast/op/op.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   func_stmt::func_stmt(std::vector<node_ptr>&& var_or_assign_decls)
   {
      make_family<op, expr>(std::move(var_or_assign_decls));
   }

   void func_stmt::accept(visitor_variant& visitor) const
   {
      using namespace mpark::patterns;
      const auto visit = [this](auto& vis) {
         vis(*this);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit);
   }

   auto func_stmt::to_string() const -> std::string { return "func_stmt"; }
} // namespace munster::ast
