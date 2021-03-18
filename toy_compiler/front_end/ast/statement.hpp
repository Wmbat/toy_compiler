#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class statement : public node
   {
   public:
      statement(const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class continue_statement : public statement
   {
   public:
      continue_statement(const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class break_statement : public statement
   {
   public:
      break_statement(const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class return_statement : public statement
   {
   public:
      return_statement(node_ptr expr, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class write_statement : public statement
   {
   public:
      write_statement(node_ptr expr, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class read_statement : public statement
   {
   public:
      read_statement(node_ptr variable, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class while_statement : public statement
   {
   public:
      while_statement(node_ptr expr, node_ptr stat_block, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class if_statement : public statement
   {
   public:
      if_statement(node_ptr expr, node_ptr stat_block_then, node_ptr stat_block_else,
                   const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class statement_block : public statement
   {
   public:
      statement_block(node_ptr node, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace fr::ast
