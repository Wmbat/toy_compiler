#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

#include <toy_compiler/munster/ast/decl/class_decl.hpp>

namespace munster::ast
{
   /*
   assert(dynamic_cast<compound_class_decl*>(compound_class.get()));   // NOLINT
   assert(dynamic_cast<compound_func_decl*>(compound_function.get())); // NOLINT
   assert(dynamic_cast<main_decl*>(main.get()));                       // NOLINT
   */

   class translation_unit_decl : public decl
   {
   public:
      translation_unit_decl(compound_class_decl::ptr compound_class, node_ptr compound_function,
                            node_ptr main);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
