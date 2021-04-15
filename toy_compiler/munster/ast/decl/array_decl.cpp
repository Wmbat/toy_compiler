#include <toy_compiler/munster/ast/decl/array_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   auto get_lexeme(node_ptr& node) -> std::string
   {
      if (node)
      {
         return std::string{node->lexeme()};
      }

      return {};
   }

   array_decl::array_decl(node_ptr start_loc, node_ptr integer_lit, node_ptr end_loc) :
      decl{get_lexeme(integer_lit), start_loc->location()},
      m_end_loc{end_loc->location()}
   {}

   auto array_decl::to_string() const -> std::string
   {
      return fmt::format("array_decl <line:{}, col:{}> <line:{}, col:{}> '[{}]'", location().line,
                         location().column, m_end_loc.line, m_end_loc.column, lexeme());
   }

   void array_decl::accept(visitor_variant& visitor) const { visit_node(visitor, *this); }

   compound_array_decl::compound_array_decl(std::vector<node_ptr>&& array_decls)
   {
      make_family<array_decl>(std::move(array_decls));
   }

   auto compound_array_decl::to_string() const -> std::string { return "compound_array_decl"; }

   void compound_array_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }
} // namespace munster::ast
