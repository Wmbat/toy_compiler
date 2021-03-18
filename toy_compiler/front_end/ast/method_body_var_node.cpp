#include <toy_compiler/front_end/ast/method_body_var_node.hpp>

namespace fr::ast
{
   method_body_var_node::method_body_var_node(node_ptr var_decl_list)
   {
      make_child(std::move(var_decl_list));
   }

   auto method_body_var_node::to_string() const -> std::string
   {
      std::string name =
         fmt::format("\"{}\"", magic_enum::enum_name(sem::action_type::method_body_var));

      if (!child())
      {
         return fmt::format("{};\n", name);
      }

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
