#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class inheritance_decl : public decl
   {
   public:
      inheritance_decl(const std::string& name, const source_location& location);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class compound_inheritance_decl : public decl
   {
   public:
      compound_inheritance_decl(std::vector<node_ptr>&& class_decls);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
