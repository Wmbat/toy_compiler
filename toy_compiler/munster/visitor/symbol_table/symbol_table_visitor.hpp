#pragma once

#include <toy_compiler/munster/visitor/visitor.hpp>

namespace munster
{
   class symbol_table_visitor : public visitor<symbol_table_visitor>
   {
      friend visitor<symbol_table_visitor>;

      struct symbol_kv
      {
         std::string key;
         symbol val;
      };

   public:
      [[nodiscard]] auto get_root_table() const -> symbol_table*;

   private:
      void visit(const ast::translation_unit_decl& node);

      void visit(const ast::compound_class_decl& node);
      void visit(const ast::class_decl& node);
      void visit(const ast::compound_inheritance_decl& node);
      void visit(const ast::inheritance_decl& node);
      void visit(const ast::compound_member_decl& node);
      void visit(const ast::member_func_decl& node);
      void visit(const ast::member_var_decl& node);

      void visit(const ast::compound_function_decl& node);
      void visit(const ast::func_decl& node);
      void visit(const ast::func_head_decl& node);
      void visit(const ast::func_body_decl& node);

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
      auto generate_var_symbols(const ast::node* head) -> std::vector<symbol>;

      void handle_parameters(std::unique_ptr<symbol_table>& p_table);
      void handle_variables(std::unique_ptr<symbol_table>& p_table);

      auto is_param_valid(const std::string_view type) -> bool;
      auto is_variable_valid(const std::string_view type) -> bool;
      auto is_pod(std::string_view type) -> bool;
      auto get_type_size(std::string_view type) -> std::int64_t;

   private:
      std::vector<std::unique_ptr<symbol_table>> m_tables;
      std::vector<symbol_kv> m_symbols;
      std::vector<std::int64_t> m_array_sizes;
   };
} // namespace munster
