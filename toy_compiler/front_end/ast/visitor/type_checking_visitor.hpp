#pragma once

#include <toy_compiler/front_end/ast/visitor/visitor.hpp>

namespace front::ast
{
   class dot_op;
   class function_expr;
   class assign_op;
   class variable_expr;

   class type_checking_visitor : public visitor
   {
   public:
      type_checking_visitor(symbol_table* p_root);

      void visit(const translation_unit_decl* tud) override;
      void visit(const compound_class_decl* ccd) override;
      void visit(const class_decl* cd) override;
      void visit(const compound_function_decl* cfd) override;
      void visit(const func_decl* fd) override;
      void visit(const func_head_decl* fbd) override;
      void visit(const func_body_decl* fbd) override;
      void visit(const compound_variable_decl*) override;
      void visit(const variable_decl*) override;
      void visit(const compound_stmt*) override;
      void visit(const main_decl*) override;
      void visit(const func_or_assign_stmt*) override;
      void visit(const return_stmt*) override;
      void visit(const write_stmt*) override;

   private:
      void check_function_decl(const func_body_decl*, std::span<symbol_table*> tables);

      void handle_assign_op(const assign_op* assign, std::span<symbol_table*> tables);
      auto handle_var_expr(const variable_expr* var_expr, std::span<symbol_table*> tables)
         -> std::string_view;
      auto handle_dot_operator(const dot_op* dot, std::span<symbol_table*> tables)
         -> std::string_view;
      auto handle_func_expr(const function_expr* func_expr, std::span<symbol_table*> tables);

      auto handle_function_expr(const function_expr*) -> std::string;
      auto handle_dot_operator(const dot_op*) -> std::string;

   private:
      symbol_table* mp_root{};
   };
} // namespace front::ast
