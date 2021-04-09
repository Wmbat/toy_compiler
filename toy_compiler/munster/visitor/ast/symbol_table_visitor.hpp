#pragma once

#include <toy_compiler/munster/visitor/visitor.hpp>

namespace munster
{
   namespace ast
   {
      class node;
   }

   class symbol_table_visitor : public visitor<symbol_table_visitor>
   {
   public:
      void visit(const ast::translation_unit_decl& node);
      void visit(const ast::compound_class_decl& node);
      void visit(const ast::class_decl& node);
      void visit(const ast::compound_function_decl& node);
      void visit(const ast::func_decl& node);
      void visit(const ast::func_head_decl& node);
      void visit(const ast::func_body_decl& node);
      void visit(const ast::compound_variable_decl& node);
      void visit(const ast::variable_decl& node);
      void visit(const ast::main_decl& node);

      void visit(const ast::compound_stmt& node);
      void visit(const ast::func_stmt& node);
      void visit(const ast::assign_stmt& node);
      void visit(const ast::if_stmt& node);
      void visit(const ast::while_stmt& node);
      void visit(const ast::read_stmt& node);
      void visit(const ast::write_stmt& node);
      void visit(const ast::return_stmt& node);
      void visit(const ast::break_stmt& node);
      void visit(const ast::continue_stmt& node);

      void visit(const ast::dot_op& node);

      [[nodiscard]] auto get_root_table() const -> symbol_table*;

   private:
      auto generate_var_symbols(const ast::node* head) -> std::vector<symbol>;

   private:
      std::vector<std::unique_ptr<symbol_table>> m_tables;
   };
} // namespace munster
