#pragma once

#include <toy_compiler/front_end/ast_bis/statement.hpp>

namespace front::ast
{
   class break_stmt : public stmt
   {
   public:
      break_stmt(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
