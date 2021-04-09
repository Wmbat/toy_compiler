#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class compound_inheritance_decl : public decl
   {
   public:
      compound_inheritance_decl(std::vector<node_ptr>&& class_decls);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
