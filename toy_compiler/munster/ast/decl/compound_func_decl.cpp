#include <toy_compiler/munster/ast/decl/compound_func_decl.hpp>

#include <toy_compiler/munster/ast/decl/func_decl.hpp>

namespace munster::ast
{
   compound_func_decl::compound_func_decl(std::vector<func_decl::ptr>&& func_decls)
   {
      make_family_e(std::move(func_decls));
   }

   void compound_func_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }
   }

   auto compound_func_decl::to_string() const -> std::string { return "compound_func_decl"; }
} // namespace munster::ast
