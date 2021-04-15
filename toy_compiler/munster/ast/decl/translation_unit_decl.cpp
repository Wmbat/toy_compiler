#include <toy_compiler/munster/ast/decl/translation_unit_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   translation_unit_decl::translation_unit_decl(compound_class_decl::ptr compound_class,
                                                node_ptr compound_function, node_ptr main)
   {
      /*
      assert(dynamic_cast<compound_class_decl*>(compound_class.get()));   // NOLINT
      assert(dynamic_cast<compound_func_decl*>(compound_function.get())); // NOLINT
      assert(dynamic_cast<main_decl*>(main.get()));                       // NOLINT
      */

      make_child(std::move(compound_class));
      make_child(std::move(compound_function));
      make_child(std::move(main));
   }

   void translation_unit_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto translation_unit_decl::to_string() const -> std::string { return "translation_unit_decl"; }
} // namespace munster::ast
