#include <toy_compiler/front_end/ast/node/compound_stmt.hpp>

namespace front::ast
{
   compound_stmt::compound_stmt(std::vector<node_ptr>&& statements)
   {
      make_family<stmt>(std::move(statements));
   }

   void compound_stmt::accept(visitor& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visitor.visit(this);
   }

   auto compound_stmt::to_string() const -> std::string { return "compound_stmt"; }
} // namespace front::ast
