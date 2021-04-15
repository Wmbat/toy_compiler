#include <toy_compiler/munster/ast/stmt/compound_stmt.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   compound_stmt::compound_stmt(std::vector<node_ptr>&& statements)
   {
      make_family<stmt>(std::move(statements));
   }

   void compound_stmt::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto compound_stmt::to_string() const -> std::string { return "compound_stmt"; }
} // namespace munster::ast
