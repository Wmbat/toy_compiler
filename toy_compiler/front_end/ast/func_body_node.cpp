#include <toy_compiler/front_end/ast/func_body_node.hpp>

namespace fr::ast
{
   func_body_node::func_body_node(node_ptr method_body_var, node_ptr statement_list)
   {
      method_body_var->make_sibling(std::move(statement_list));
      make_child(std::move(method_body_var));
   }

   auto func_body_node::to_string() const -> std::string
   {
      return fmt::format("{}", sem::to_string_view(sem::action_type::func_body));
   }
} // namespace fr::ast
