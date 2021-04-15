#include <toy_compiler/munster/ast/decl/stmt_block_decl.hpp>

namespace munster::ast
{
   stmt_block_decl::stmt_block_decl(node_ptr node)
   {
      if (node)
      {
         make_child(std::move(node));
      }
   }

   auto stmt_block_decl::to_string() const -> std::string { return "stmt_block_decl"; }
} // namespace munster::ast
