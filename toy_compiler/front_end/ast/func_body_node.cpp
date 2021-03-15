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
      std::string name = fmt::format("\"{}\"", sem::to_string_view(sem::action_type::func_body));

      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      return fmt::format("{};\n{}", name, output);
   }
} // namespace fr::ast
