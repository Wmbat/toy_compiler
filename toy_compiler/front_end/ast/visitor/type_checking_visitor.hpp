#pragma once

#include <toy_compiler/front_end/ast/visitor/visitor.hpp>

namespace front::ast
{
   class type_checking_visitor : public visitor
   {
   public:
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

   private:
      std::vector<std::unique_ptr<symbol_table>> m_tables;
   };
} // namespace front::ast
