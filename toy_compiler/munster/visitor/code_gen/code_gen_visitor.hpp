#pragma once

#include <toy_compiler/munster/visitor/visitor.hpp>

namespace munster
{
   class code_gen_visitor : public visitor<code_gen_visitor>
   {
   public:
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

      void visit(const ast::compound_variable_decl& node);
      void visit(const ast::variable_decl& node);

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
   };
} // namespace munster
