#include <toy_compiler/front_end/ast/program_node.hpp>

namespace fr::ast
{
   program_node::program_node(node_ptr class_decl_list, node_ptr func_def_list, node_ptr func_body)
   {
      func_def_list->make_sibling(std::move(func_body));
      class_decl_list->make_sibling(std::move(func_def_list));

      make_child(std::move(class_decl_list));
   }

   auto program_node::to_string() const -> std::string
   {
      std::string name = fmt::format("\"{}\"", magic_enum::enum_name(sem::action_type::program));

      if (!child())
      {
         return name;
      }

      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      return output;
   }
} // namespace fr::ast
