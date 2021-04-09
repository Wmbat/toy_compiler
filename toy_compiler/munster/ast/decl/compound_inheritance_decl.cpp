#include <toy_compiler/munster/ast/decl/compound_inheritance_decl.hpp>

#include <toy_compiler/munster/ast/decl/inheritance_decl.hpp>

namespace munster::ast
{
   compound_inheritance_decl::compound_inheritance_decl(std::vector<node_ptr>&& inheritance_decls)
   {
      make_family<inheritance_decl>(std::move(inheritance_decls));
   }

   auto compound_inheritance_decl::to_string() const -> std::string
   {
      return "compound_inheritance_decl";
   }
} // namespace munster::ast
