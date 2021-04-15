#include <toy_compiler/munster/ast/decl/func_decl.hpp>

#include <toy_compiler/munster/ast/decl/func_body_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_head_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   func_decl::func_decl(node_ptr function_head, node_ptr function_body)
   {
      assert(dynamic_cast<func_head_decl*>(function_head.get())); // NOLINT
      assert(dynamic_cast<func_body_decl*>(function_body.get())); // NOLINT

      make_child(std::move(function_head));
      make_child(std::move(function_body));
   }

   void func_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto func_decl::to_string() const -> std::string { return "func_decl"; }

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
