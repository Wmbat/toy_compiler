#include <toy_compiler/munster/ast/decl/inheritance_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   inheritance_decl::inheritance_decl(const std::string& name, const source_location& location) :
      decl{name, location}
   {}

   void inheritance_decl::accept(visitor_variant& visitor) const { visit_node(visitor, *this); }

   auto inheritance_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "inheritance_decl", location().line,
                         location().column, lexeme());
   }

   compound_inheritance_decl::compound_inheritance_decl(std::vector<node_ptr>&& inheritance_decls)
   {
      make_family<inheritance_decl>(std::move(inheritance_decls));
   }

   void compound_inheritance_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto compound_inheritance_decl::to_string() const -> std::string
   {
      return "compound_inheritance_decl";
   }
} // namespace munster::ast
