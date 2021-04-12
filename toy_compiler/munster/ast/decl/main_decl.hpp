#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

#include <toy_compiler/munster/ast/decl/func_body_decl.hpp>
#include <toy_compiler/munster/ast/decl/id_decl.hpp>

namespace munster::ast
{
   class main_decl : public decl
   {
   public:
      main_decl(id_decl::ptr id, func_body_decl::ptr func_body);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
