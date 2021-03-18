#include <toy_compiler/front_end/ast/method_body_var_node.hpp>

namespace fr::ast
{
   static inline std::uint32_t counter = 0;

   method_body_var_node::method_body_var_node(node_ptr var_decl_list) : node{{}, {}, counter++}
   {
      make_child(std::move(var_decl_list));
   }

   auto method_body_var_node::to_string() const -> std::string
   {
      std::string name = fmt::format(
         "\"{}_{}\"", magic_enum::enum_name(sem::action_type::method_body_var), index());

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

      output += fmt::format("{} [label={}]\n", name,
                            magic_enum::enum_name(sem::action_type::method_body_var));

      return fmt::format("{};\n{}", name, output);
   }
} // namespace fr::ast
