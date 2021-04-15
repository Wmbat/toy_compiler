#include <toy_compiler/munster/ast/op/assign_op.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

#include <fmt/core.h>

#include <cassert>
#include <utility>

namespace munster::ast
{
   assign_op::assign_op(node_ptr val_0, node_ptr id_decl, node_ptr val_1) :
      op{std::string{id_decl->lexeme()}, id_decl->location()}
   {
      make_child(std::move(val_0));
      make_child(std::move(val_1));
   }

   void assign_op::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto assign_op::to_string() const -> std::string
   {
      return fmt::format("operator{} {}", lexeme(), location());
   }
} // namespace munster::ast
