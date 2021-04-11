#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class compound_member_decl : public decl
   {
   public:
      compound_member_decl(std::vector<node_ptr>&& member_decl);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
