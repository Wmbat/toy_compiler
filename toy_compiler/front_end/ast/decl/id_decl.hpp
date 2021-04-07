#pragma once

#include <toy_compiler/front_end/ast/decl/decl.hpp>

namespace front::ast
{
   class id_decl : public decl
   {
   public:
      id_decl(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
