#include <toy_compiler/munster/ast/decl/compound_member_decl.hpp>

#include <toy_compiler/munster/ast/decl/member_func_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_var_decl.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   compound_member_decl::compound_member_decl(std::vector<node_ptr>&& member_decls)
   {
      make_family<member_func_decl, member_var_decl>(std::move(member_decls));
   }

   void compound_member_decl::accept(visitor_variant& visitor) const
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

   auto compound_member_decl::to_string() const -> std::string { return "compound_member_decl"; }
} // namespace munster::ast
