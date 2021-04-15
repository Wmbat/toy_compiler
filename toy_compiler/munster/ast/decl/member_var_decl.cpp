#include <toy_compiler/munster/ast/decl/member_var_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   member_var_decl::member_var_decl(node_ptr visibility, node_ptr type, node_ptr id,
                                    node_ptr compound_array) :
      decl{std::string{id->lexeme()}, type->location()},
      m_type{type->lexeme()}
   {
      if (visibility)
      {
         m_visibility = std::string{visibility->lexeme()};
      }

      if (!std::empty(compound_array->children()))
      {
         make_child(std::move(compound_array));
      }
   }

   auto member_var_decl::visibility() const -> std::string_view { return m_visibility; }
   auto member_var_decl::type() const -> std::string_view { return m_type; }

   auto member_var_decl::to_string() const -> std::string
   {
      return fmt::format("member_var_decl {} {} '{} {}'", location(), lexeme(), visibility(),
                         type());
   }

   void member_var_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }
} // namespace munster::ast
