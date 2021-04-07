#include <toy_compiler/front_end/ast/decl/compound_class_decl.hpp>

namespace front::ast
{
   compound_class_decl::compound_class_decl(std::vector<class_decl::ptr>&& class_decls)
   {
      make_family_e<class_decl>(std::move(class_decls));
   };

   void compound_class_decl::accept(visitor& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }
   }

   auto compound_class_decl::to_string() const -> std::string { return "compound_class_decl"; }
} // namespace front::ast
