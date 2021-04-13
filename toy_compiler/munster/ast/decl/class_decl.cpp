#include <toy_compiler/munster/ast/decl/class_decl.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   class_decl::class_decl(node_ptr class_start, node_ptr class_name, node_ptr compound_inheritance,
                          node_ptr compound_member) :
      decl{class_name->to_string(), class_start->location()}
   {
      /*
      assert(dynamic_cast<location_decl*>(class_start.get()));                      // NOLINT
      assert(dynamic_cast<id_decl*>(class_name.get()));                             // NOLINT
      assert(dynamic_cast<compound_inheritance_decl*>(compound_inheritance.get())); // NOLINT
      assert(dynamic_cast<compound_member_decl*>(compound_member.get()));           // NOLINT
      */

      if (!std::empty(compound_inheritance->children()))
      {
         make_child(std::move(compound_inheritance));
      }

      if (!std::empty(compound_member->children()))
      {
         make_child(std::move(compound_member));
      }
   }

   auto class_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "class_decl", location().line,
                         location().column, lexeme());
   }

   void class_decl::accept(visitor_variant& visitor) const
   {
      using namespace mpark::patterns;

      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      const auto visit = [this](auto& vis) {
         vis(*this);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit);
   }

   compound_class_decl::compound_class_decl(std::vector<class_decl::ptr>&& class_decls)
   {
      make_family_e<class_decl>(std::move(class_decls));
   };

   void compound_class_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }
   }

   auto compound_class_decl::to_string() const -> std::string { return "compound_class_decl"; }
} // namespace munster::ast
