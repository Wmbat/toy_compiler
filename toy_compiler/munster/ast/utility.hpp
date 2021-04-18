#pragma once

#include <toy_compiler/munster/ast/node.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   template <typename Any>
   void visit_node(visitor_variant& visitor, const Any& node)
   {
      using namespace mpark::patterns;

      const auto visit = [&](auto& vis) {
         vis(node);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<memory_size_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit,
                     pattern(as<code_gen_visitor>(arg)) = visit);
   }
} // namespace munster::ast
