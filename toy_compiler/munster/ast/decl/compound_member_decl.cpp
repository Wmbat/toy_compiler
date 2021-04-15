#include <toy_compiler/munster/ast/decl/compound_member_decl.hpp>

#include <toy_compiler/munster/ast/decl/member_func_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_var_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   compound_member_decl::compound_member_decl(std::vector<node_ptr>&& member_decls)
   {
      make_family<member_func_decl, member_var_decl>(std::move(member_decls));
   }

   void compound_member_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto compound_member_decl::to_string() const -> std::string { return "compound_member_decl"; }
} // namespace munster::ast
