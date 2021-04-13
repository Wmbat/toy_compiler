#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <toy_compiler/munster/ast/decl/func_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_func_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_var_decl.hpp>
#include <toy_compiler/munster/ast/decl/variable_decl.hpp>
#include <toy_compiler/munster/ast/stmt/compound_stmt.hpp>

#include <toy_compiler/munster/visitor/visitor.hpp>

#include <cassert>
#include <utility>

namespace munster::ast
{
   compound_array_index_access_decl::compound_array_index_access_decl(
      std::vector<node_ptr>&& variables)
   {
      make_family<array_index_access_decl>(std::move(variables));
   }

   array_index_access_decl::array_index_access_decl(node_ptr beg, node_ptr expr, node_ptr end) :
      decl{beg->location()},
      m_end{end->location()}
   {
      /*
      assert(dynamic_cast<location_decl*>(beg.get())); // NOLINT
      assert(dynamic_cast<location_decl*>(end.get())); // NOLINT
      */

      make_child(std::move(expr));
   }

   auto compound_array_index_access_decl::to_string() const -> std::string
   {
      return "compound_array_index_access_decl";
   }

   auto array_index_access_decl::to_string() const -> std::string
   {
      return fmt::format("array_index_access_decl <line:{}, col:{}> <line:{}, col:{}>",
                         location().line, location().column, m_end.line, m_end.column);
   }

   stmt_block_decl::stmt_block_decl(node_ptr node)
   {
      if (node)
      {
         make_child(std::move(node));
      }
   }

   auto stmt_block_decl::to_string() const -> std::string { return "stmt_block_decl"; }

}; // namespace munster::ast
