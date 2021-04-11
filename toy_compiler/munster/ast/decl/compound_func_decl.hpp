#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>
#include <toy_compiler/munster/ast/decl/func_decl.hpp>

namespace munster::ast
{
   class compound_func_decl : public decl
   {
   public:
      compound_func_decl(std::vector<func_decl::ptr>&& class_decls);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast