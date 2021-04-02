#pragma once

#include <toy_compiler/front_end/ast/symbol_table.hpp>
#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <range/v3/algorithm/find.hpp>

#include <cassert>
#include <variant>
#include <vector>

namespace front::ast
{
   class visitor
   {
   public:
      void visit(const translation_unit_decl& tl);
      void visit(const class_decl& cd);

   private:
      std::vector<std::unique_ptr<symbol_table>> m_tables;
   };
}; // namespace front::ast
