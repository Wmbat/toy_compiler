#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class id_decl : public decl
   {
   public:
      using ptr = std::unique_ptr<id_decl>;

   public:
      id_decl(const std::string& lexeme, const front::source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
