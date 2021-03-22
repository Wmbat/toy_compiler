#pragma once

#include <toy_compiler/front_end/ast_bis/node.hpp>

namespace front::ast
{
   class stmt : public node
   {
   public:
      stmt() = default;
      stmt(const source_location& location);
      stmt(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class assign_stmt : public stmt
   {
   };

   class if_stmt : public stmt
   {
   };

   class while_stmt : public stmt
   {
   };

   class read_stmt : public stmt
   {
   };

   class write_stmt : public stmt
   {
   };

   class return_stmt : public stmt
   {
   };

   class break_stmt : public stmt
   {
   };

   class continue_stmt : public stmt
   {
   };
} // namespace front::ast
