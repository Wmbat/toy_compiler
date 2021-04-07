#include <toy_compiler/front_end/ast/decl/func_decl.hpp>

#include <toy_compiler/front_end/ast/decl/func_body_decl.hpp>
#include <toy_compiler/front_end/ast/decl/func_head_decl.hpp>

namespace front::ast
{
   func_decl::func_decl(node_ptr function_head, node_ptr function_body)
   {
      assert(dynamic_cast<func_head_decl*>(function_head.get())); // NOLINT
      assert(dynamic_cast<func_body_decl*>(function_body.get())); // NOLINT

      make_child(std::move(function_head));
      make_child(std::move(function_body));
   }

   void func_decl::accept(visitor& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visitor.visit(this);
   }

   auto func_decl::to_string() const -> std::string { return "func_decl"; }
} // namespace front::ast
