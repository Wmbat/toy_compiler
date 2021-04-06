#include <toy_compiler/front_end/ast/node/func_body_decl.hpp>

namespace front::ast
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

   void func_body_decl::accept(visitor& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visitor.visit(this);
   }

   auto func_body_decl::to_string() const -> std::string { return "func_body_decl"; }
} // namespace front::ast
