#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

#include <toy_compiler/munster/source_location.hpp>

namespace munster::ast
{
   class compound_params_decl : public decl
   {
   public:
      compound_params_decl(std::vector<node_ptr>&& member_decl);

      void accept(visitor_variant &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
