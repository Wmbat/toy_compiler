#pragma once

#include <toy_compiler/munster/visitor/visitor.hpp>

#include <stack>
#include <vector>

namespace munster
{
   class type_checking_visitor : public visitor<type_checking_visitor>
   {
      friend visitor<type_checking_visitor>;

   public:
      type_checking_visitor(symbol_table* p_root);

      void visit(const ast::translation_unit_decl&);

      void visit(const ast::compound_class_decl&);
      void visit(const ast::class_decl&);
      void visit(const ast::compound_inheritance_decl& node);
      void visit(const ast::inheritance_decl& node);
      void visit(const ast::compound_member_decl& node);
      void visit(const ast::member_func_decl& node);
      void visit(const ast::member_var_decl& node);

      void visit(const ast::compound_function_decl&);
      void visit(const ast::func_decl&);
      void visit(const ast::func_head_decl&);
      void visit(const ast::func_body_decl&);

      void visit(const ast::main_decl& node);

      void visit(const ast::compound_params_decl& node);
      void visit(const ast::compound_variable_decl& node);
      void visit(const ast::variable_decl& node);
      void visit(const ast::compound_array_decl& node);
      void visit(const ast::array_decl& node);

      void visit(const ast::func_expr& node);
      void visit(const ast::var_expr& node);
      void visit(const ast::float_expr& node);
      void visit(const ast::integer_expr& node);
      void visit(const ast::string_expr& node);

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
      void visit(const ast::assign_op& node);
      void visit(const ast::add_op& node);
      void visit(const ast::mult_op& node);
      void visit(const ast::rel_op& node);

   private:
      auto get_variable_type(const symbol* p_symbol) const -> std::string;
      auto is_type_error(const symbol* p_symbol) const -> bool;

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
      inline static std::int64_t m_temporary_counter = 0; // NOLINT

      symbol_table* mp_root{};

      std::vector<symbol_table*> m_tables;
      std::vector<symbol*> m_symbols;
   };
} // namespace munster
