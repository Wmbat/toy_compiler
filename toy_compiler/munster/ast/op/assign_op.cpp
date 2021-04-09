#include <toy_compiler/munster/ast/op/assign_op.hpp>

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

   auto assign_op::to_string() const -> std::string
   {
      return fmt::format("operator{} {}", lexeme(), location());
   }
} // namespace munster::ast
