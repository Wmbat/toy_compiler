#include <toy_compiler/front_end/ast/method_body_var_node.hpp>

namespace fr::ast
{
   method_body_var_node::method_body_var_node(node_ptr var_decl_list)
   {
      make_child(std::move(var_decl_list));
   }

   auto method_body_var_node::to_string() const -> std::string
   {
      return fmt::format("{}", sem::to_string_view(sem::action_type::method_body_var));
   }
} // namespace fr::ast
