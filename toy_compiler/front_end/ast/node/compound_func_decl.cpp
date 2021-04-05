#include <toy_compiler/front_end/ast/node/compound_func_decl.hpp>

#include <toy_compiler/front_end/ast/node/func_decl.hpp>

namespace front::ast
{
   compound_func_decl::compound_func_decl(std::vector<node_ptr>&& func_decls)
   {
      make_family<func_decl>(std::move(func_decls));
   }

   void compound_func_decl::accept(visitor& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }
   }

   auto compound_func_decl::to_string() const -> std::string { return "compound_func_decl"; }
} // namespace front::ast
