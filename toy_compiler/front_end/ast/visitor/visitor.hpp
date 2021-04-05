#pragma once

#include <toy_compiler/front_end/ast/symbol_table.hpp>

#include <toy_compiler/front_end/parse_error.hpp>
#include <toy_compiler/util/strong_type.hpp>

#include <range/v3/algorithm/find.hpp>

#include <cassert>
#include <variant>
#include <vector>

namespace front::ast
{
   class translation_unit_decl;
   class compound_class_decl;
   class class_decl;
   class compound_function_decl;
   class func_decl;
   class func_head_decl;
   class func_body_decl;
   class compound_variable_decl;
   class variable_decl;
   class compound_stmt;
   class main_decl;

   class visitor
   {
   public:
      virtual void visit(const translation_unit_decl* tud) = 0;
      virtual void visit(const compound_class_decl* ccd) = 0;
      virtual void visit(const class_decl* cd) = 0;
      virtual void visit(const compound_function_decl* cfd) = 0;
      virtual void visit(const func_decl* fd) = 0;
      virtual void visit(const func_head_decl*) = 0;
      virtual void visit(const func_body_decl*) = 0;
      virtual void visit(const compound_variable_decl*) = 0;
      virtual void visit(const variable_decl*) = 0;
      virtual void visit(const compound_stmt*) = 0;
      virtual void visit(const main_decl*) = 0;

      [[nodiscard]] auto get_errors() const -> const std::vector<parse_error>& { return m_errors; }

   protected:
      std::vector<parse_error> m_errors; // NOLINT
   };
}; // namespace front::ast
