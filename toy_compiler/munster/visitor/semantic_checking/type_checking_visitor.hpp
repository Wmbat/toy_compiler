#pragma once

#include <toy_compiler/munster/visitor/visitor.hpp>

#include <stack>
#include <vector>

namespace munster
{
   class type_checking_visitor : public visitor<type_checking_visitor>
   {
   public:
      type_checking_visitor(symbol_table* p_root);

      void visit(const ast::translation_unit_decl&);
      void visit(const ast::compound_class_decl&);
      void visit(const ast::class_decl&);
      void visit(const ast::compound_function_decl&);
      void visit(const ast::func_decl&);
      void visit(const ast::func_head_decl&);
      void visit(const ast::func_body_decl&);

      void visit(const ast::func_expr& node);
      void visit(const ast::var_expr& node);

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

      void visit(const ast::compound_variable_decl& node);
      void visit(const ast::variable_decl& node);
      void visit(const ast::main_decl& node);

   private:
      void check_function_decl(const ast::func_body_decl*, std::span<symbol_table*> tables);

      void handle_assign_op(const ast::assign_op* assign, std::span<symbol_table*> tables);
      auto handle_var_expr(const ast::var_expr* var_expr, std::span<symbol_table*> tables)
         -> std::string_view;
      auto handle_dot_operator(const ast::dot_op* dot, std::span<symbol_table*> tables)
         -> std::string_view;
      auto handle_func_expr(const ast::func_expr* func_expr, std::span<symbol_table*> tables);

      auto handle_function_expr(const ast::func_expr*) -> std::string;
      auto handle_dot_operator(const ast::dot_op*) -> std::string;

   private:
      symbol_table* mp_root{};

      std::vector<symbol_table*> m_tables;
      std::vector<symbol*> m_symbols;
   };
} // namespace munster
