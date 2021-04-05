#include <toy_compiler/front_end/ast/visitor/type_checking_visitor.hpp>

#include <toy_compiler/front_end/ast/node/func_body_decl.hpp>
#include <toy_compiler/front_end/ast/node/func_decl.hpp>
#include <toy_compiler/front_end/ast/node/func_head_decl.hpp>
#include <toy_compiler/front_end/ast/node/member_func_decl.hpp>
#include <toy_compiler/front_end/ast/node/member_var_decl.hpp>

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

namespace front::ast
{
   void type_checking_visitor::visit(const translation_unit_decl* /*tud*/) {}
   void type_checking_visitor::visit(const compound_class_decl* /*ccd*/) {}
   void type_checking_visitor::visit(const class_decl* /*cd*/) {}
   void type_checking_visitor::visit(const compound_function_decl* /*cfd*/) {}
   void type_checking_visitor::visit(const func_decl* /*fd*/) {}
   void type_checking_visitor::visit(const func_head_decl*) {}
   void type_checking_visitor::visit(const func_body_decl*) {}
   void type_checking_visitor::visit(const compound_variable_decl*) {}
   void type_checking_visitor::visit(const variable_decl*) {}
   void type_checking_visitor::visit(const compound_stmt*) {}
   void type_checking_visitor::visit(const main_decl*) {}
} // namespace front::ast
