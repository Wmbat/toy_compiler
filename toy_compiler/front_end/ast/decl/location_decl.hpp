#pragma once

#include <toy_compiler/front_end/ast/decl/decl.hpp>

namespace front::ast
{
   class location_decl : public decl
   {
   public:
      using ptr = std::unique_ptr<location_decl>;

   public:
      location_decl(const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
