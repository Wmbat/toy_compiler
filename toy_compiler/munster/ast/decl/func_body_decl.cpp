#include <toy_compiler/munster/ast/decl/func_body_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   func_body_decl::func_body_decl(node_ptr variables, node_ptr statements)
   {
      if (!std::empty(variables->children()))
      {
         make_child(std::move(variables));
      }

      if (!std::empty(statements->children()))
      {
         make_child(std::move(statements));
      }
   }

   void func_body_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto func_body_decl::to_string() const -> std::string { return "func_body_decl"; }
} // namespace munster::ast
