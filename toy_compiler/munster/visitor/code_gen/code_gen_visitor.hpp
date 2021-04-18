#pragma once

#include <toy_compiler/munster/visitor/visitor.hpp>

#include <toy_compiler/munster/visitor/code_gen/register_pool.hpp>

namespace munster
{
   class code_gen_visitor : public visitor<code_gen_visitor>
   {
      friend visitor<code_gen_visitor>;

   public:
      code_gen_visitor(symbol_table* p_root);

      void place_marker(std::string marker);

      auto moon_code() -> std::string_view;

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
      void visit(const ast::stmt_block_decl& node);

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
      std::string moon_code_output;
      const std::string spacing{"          "};

      std::int64_t m_literal_counter = 0;
      std::int64_t m_temporary_counter = 0;
      std::int64_t m_if_counter = 0;

      symbol_table* mp_root{};

      std::vector<symbol_table*> m_tables;
      std::vector<symbol*> m_symbols;
      std::vector<std::string> m_blocks;

      register_pool m_register_pool;
   };
} // namespace munster
