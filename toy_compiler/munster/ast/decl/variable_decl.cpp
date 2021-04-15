#include <toy_compiler/munster/ast/decl/variable_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   variable_decl::variable_decl(node_ptr type, node_ptr id, node_ptr compound_array_decl) :
      decl{std::string{id->lexeme()}, type->location()},
      m_type(type->lexeme())
   {
      if (!std::empty(compound_array_decl->children()))
      {
         make_child(std::move(compound_array_decl));
      }
   }

   auto variable_decl::type() const -> std::string_view { return m_type; }
   auto variable_decl::to_string() const -> std::string
   {
      return fmt::format("variable_decl <line:{}, col:{}> {} '{}'", location().line,
                         location().column, lexeme(), m_type);
   }

   void variable_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   compound_variable_decl::compound_variable_decl(std::vector<node_ptr>&& variables)
   {
      make_family<variable_decl>(std::move(variables));
   }

   void compound_variable_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto compound_variable_decl::to_string() const -> std::string
   {
      return fmt::format("compound_variable_decl");
   }
} // namespace munster::ast
