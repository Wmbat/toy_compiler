#include <toy_compiler/munster/ast/expr/var_expr.hpp>

#include <mpark/patterns.hpp>

using namespace mpark::patterns;

namespace munster::ast
{
   var_expr::var_expr(node_ptr id, node_ptr compound_array_indices) :
      expr{std::string{id->lexeme()}, id->location()}
   {
      if (!std::empty(compound_array_indices->children()))
      {
         make_child(std::move(compound_array_indices));
      }
   }

   void var_expr::accept(visitor_variant& visitor) const
   {
      const auto visit = [this](auto& vis) {
         vis(*this);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit);
   }

   auto var_expr::to_string() const -> std::string
   {
      return fmt::format("var_expr <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace munster::ast
