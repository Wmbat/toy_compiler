#include <toy_compiler/munster/ast/expr/ternary_expr.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   ternary_expr::ternary_expr(node_ptr location, node_ptr condition, node_ptr expr_0,
                              node_ptr expr_1) :
      expr{location->location()}
   {
      make_child(std::move(condition));
      make_child(std::move(expr_0));
      make_child(std::move(expr_1));
   }

   auto ternary_expr::to_string() const -> std::string
   {
      return fmt::format("ternary_expr <line:{}, col:{}>", location().line, location().column);
   }
} // namespace munster::ast
