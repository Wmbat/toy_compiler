#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class type_decl : public decl
   {
   public:
      type_decl(const std::string& name, const front::source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
