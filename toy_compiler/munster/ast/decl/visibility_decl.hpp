#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class visibility_decl : public decl
   {
   public:
      visibility_decl(const std::string& name, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
