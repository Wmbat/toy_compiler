#include <toy_compiler/munster/ast/stmt/if_stmt.hpp>

#include <toy_compiler/front_end/ast_bis/declaration.hpp>
#include <toy_compiler/munster/ast/decl/id_decl.hpp>
#include <toy_compiler/munster/ast/decl/location_decl.hpp>
#include <toy_compiler/munster/ast/expr/expr.hpp>
#include <toy_compiler/munster/ast/op/op.hpp>

namespace munster::ast
{
   if_stmt::if_stmt(node_ptr loc, node_ptr expr_node, node_ptr then_block, node_ptr else_block) :
      stmt{loc->location()}
   {
      { // ASSERTION BLOCK
         using detail::is_type;

         const bool location_assert = is_type<location_decl>(loc);
         const bool expr_assert = is_type<expr, op>(expr_node);
         const bool then_assert = is_type<stmt_block_decl>(then_block);
         const bool else_assert = is_type<stmt_block_decl>(else_block);

         assert(location_assert && expr_assert && then_assert && else_assert); // NOLINT
      }

      make_child(std::move(expr_node));
      make_child(std::move(then_block));
      make_child(std::move(else_block));
   }

   auto if_stmt::to_string() const -> std::string
   {
      return fmt::format("if_stmt <line:{}, col:{}>", location().line, location().column);
   }
} // namespace munster::ast
