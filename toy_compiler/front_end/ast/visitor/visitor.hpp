#pragma once

#include <toy_compiler/front_end/ast/symbol_table.hpp>
#include <toy_compiler/front_end/ast_bis/declaration.hpp>
#include <toy_compiler/front_end/ast_bis/function_decl.hpp>
#include <toy_compiler/front_end/parse_error.hpp>
#include <toy_compiler/util/strong_type.hpp>

#include <range/v3/algorithm/find.hpp>

#include <cassert>
#include <variant>
#include <vector>

namespace front::ast
{
   using node_variant = std::variant<const translation_unit_decl*, const compound_class_decl*,
                                     const class_decl*, const compound_function_decl*>;

   class visitor
   {
   public:
      void operator()(const translation_unit_decl* tud);
      void operator()(const compound_class_decl* ccd);
      void operator()(const class_decl* cd);
      void operator()(const compound_function_decl* cfd);

      auto get_errors() const -> const std::vector<parse_error>&; // NOLINT

   private:
      std::vector<std::unique_ptr<symbol_table>> m_tables;

      std::vector<parse_error> m_errors;
   };
}; // namespace front::ast
