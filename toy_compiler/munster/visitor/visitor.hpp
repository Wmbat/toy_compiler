#pragma once

#include <toy_compiler/front_end/parse_error.hpp>
#include <toy_compiler/munster/symbol_table.hpp>
#include <toy_compiler/util/crtp.hpp>
#include <toy_compiler/util/strong_type.hpp>

#include <range/v3/algorithm/find.hpp>

#include <cassert>
#include <variant>
#include <vector>

namespace munster
{
   namespace ast
   {
      class node;

      class translation_unit_decl;

      class compound_class_decl;
      class class_decl;
      class compound_inheritance_decl;
      class compound_member_decl;
      class member_func_decl;
      class member_var_decl;

      class compound_function_decl;
      class inheritance_decl;
      class func_decl;
      class func_head_decl;
      class func_body_decl;

      class func_expr;
      class var_expr;
      class float_expr;
      class integer_expr;
      class string_expr;

      class compound_stmt;
      class func_stmt;
      class assign_stmt;
      class if_stmt;
      class while_stmt;
      class read_stmt;
      class return_stmt;
      class write_stmt;
      class break_stmt;
      class continue_stmt;

      class dot_op;
      class assign_op;
      class add_op;
      class mult_op;
      class rel_op;

      class compound_variable_decl;
      class variable_decl;

      class main_decl;
   } // namespace ast

   template <typename Any>
   class visitor : public util::crtp<Any, visitor>
   {
   public:
      void operator()(const ast::translation_unit_decl& tl) { this->underlying().visit(tl); }

      void operator()(const ast::compound_class_decl& node) { this->underlying().visit(node); }
      void operator()(const ast::class_decl& node) { this->underlying().visit(node); }
      void operator()(const ast::compound_inheritance_decl& node)
      {
         this->underlying().visit(node);
      }
      void operator()(const ast::inheritance_decl& node) { this->underlying().visit(node); }
      void operator()(const ast::compound_member_decl& node) { this->underlying().visit(node); }
      void operator()(const ast::member_func_decl& node) { this->underlying().visit(node); }
      void operator()(const ast::member_var_decl& node) { this->underlying().visit(node); }

      void operator()(const ast::compound_function_decl& cfd) { this->underlying().visit(cfd); }
      void operator()(const ast::func_decl& fd) { this->underlying().visit(fd); }
      void operator()(const ast::func_head_decl& fd) { this->underlying().visit(fd); }
      void operator()(const ast::func_body_decl& fbd) { this->underlying().visit(fbd); }
      void operator()(const ast::compound_variable_decl& cv) { this->underlying().visit(cv); }
      void operator()(const ast::variable_decl& var) { this->underlying().visit(var); }
      void operator()(const ast::main_decl& main) { this->underlying().visit(main); }

      void operator()(const ast::func_expr& node) { this->underlying().visit(node); }
      void operator()(const ast::var_expr& node) { this->underlying().visit(node); }
      void operator()(const ast::float_expr& node) { this->underlying().visit(node); }
      void operator()(const ast::integer_expr& node) { this->underlying().visit(node); }
      void operator()(const ast::string_expr& node) { this->underlying().visit(node); }

      void operator()(const ast::compound_stmt& node) { this->underlying().visit(node); }
      void operator()(const ast::func_stmt& node) { this->underlying().visit(node); }
      void operator()(const ast::assign_stmt& node) { this->underlying().visit(node); }
      void operator()(const ast::if_stmt& node) { this->underlying().visit(node); }
      void operator()(const ast::while_stmt& node) { this->underlying().visit(node); }
      void operator()(const ast::read_stmt& node) { this->underlying().visit(node); }
      void operator()(const ast::write_stmt& node) { this->underlying().visit(node); }
      void operator()(const ast::return_stmt& node) { this->underlying().visit(node); }
      void operator()(const ast::break_stmt& node) { this->underlying().visit(node); }
      void operator()(const ast::continue_stmt& node) { this->underlying().visit(node); }

      void operator()(const ast::dot_op& node) { this->underlying().visit(node); }
      void operator()(const ast::assign_op& node) { this->underlying().visit(node); }
      void operator()(const ast::add_op& node) { this->underlying().visit(node); }
      void operator()(const ast::mult_op& node) { this->underlying().visit(node); }
      void operator()(const ast::rel_op& node) { this->underlying().visit(node); }

      [[nodiscard]] auto get_errors() const -> const std::vector<front::parse_error>&
      {
         return m_errors;
      }

   protected:
      std::vector<front::parse_error> m_errors; // NOLINT
   };
}; // namespace munster
