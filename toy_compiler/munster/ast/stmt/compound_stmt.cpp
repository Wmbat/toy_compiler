#include <toy_compiler/munster/ast/stmt/compound_stmt.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   compound_stmt::compound_stmt(std::vector<node_ptr>&& statements)
   {
      make_family<stmt>(std::move(statements));
   }

   void compound_stmt::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      using namespace mpark::patterns;
      const auto visit = [this](auto& vis) {
         vis(*this);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit);
   }

   auto compound_stmt::to_string() const -> std::string { return "compound_stmt"; }
} // namespace munster::ast
