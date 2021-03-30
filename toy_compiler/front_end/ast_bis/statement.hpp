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
   public:
      write_stmt(node_ptr value, node_ptr expr);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class return_stmt : public stmt
   {
   public:
      return_stmt(node_ptr value, node_ptr expr);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class break_stmt : public stmt
   {
   public:
      break_stmt(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class continue_stmt : public stmt
   {
   public:
      continue_stmt(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
