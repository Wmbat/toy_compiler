#include <toy_compiler/front_end/ast_bis/compound_stmt.hpp>

namespace front::ast
{
   compound_stmt::compound_stmt(std::vector<node_ptr>&& statements)
   {
      make_family<stmt>(std::move(statements));
   }

   auto compound_stmt::to_string() const -> std::string { return "compound_stmt"; }
} // namespace front::ast
