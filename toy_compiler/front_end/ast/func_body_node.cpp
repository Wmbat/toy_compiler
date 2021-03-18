#include <toy_compiler/front_end/ast/func_body_node.hpp>

namespace fr::ast
{
   static inline std::uint32_t counter = 0;

   function::function(node_ptr func_head, node_ptr func_body, const source_location& location) :
      node{{}, location, counter++}
   {
      func_head->make_sibling(std::move(func_body));
      make_child(std::move(func_head));
   }

   auto function::to_string() const -> std::string
   {
      std::string name =
         fmt::format("\"{}_{}\"", magic_enum::enum_name(sem::action_type::function), index());
      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output +=
         fmt::format("{} [label={}]\n", name, magic_enum::enum_name(sem::action_type::function));

      return fmt::format("{};\n{}", name, output);
   }

   func_head::func_head(node_ptr id, node_ptr class_method, node_ptr function_param,
                        node_ptr return_type, const source_location& location) :
      node{{}, location, counter++}
   {
      function_param->make_sibling(std::move(return_type));
      class_method->make_sibling(std::move(function_param));
      id->make_sibling(std::move(class_method));
      make_child(std::move(id));
   }

   auto func_head::to_string() const -> std::string
   {
      std::string name =
         fmt::format("\"{}_{}\"", magic_enum::enum_name(sem::action_type::func_head), index());
      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output +=
         fmt::format("{} [label={}]\n", name, magic_enum::enum_name(sem::action_type::func_head));

      return fmt::format("{};\n{}", name, output);
   }

   func_body_node::func_body_node(node_ptr method_body_var, node_ptr statement_list) :
      node{{}, {}, counter++}
   {
      method_body_var->make_sibling(std::move(statement_list));
      make_child(std::move(method_body_var));
   }

   auto func_body_node::to_string() const -> std::string
   {
      std::string name =
         fmt::format("\"{}_{}\"", magic_enum::enum_name(sem::action_type::func_body), index());
      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output +=
         fmt::format("{} [label={}]\n", name, magic_enum::enum_name(sem::action_type::func_body));

      return fmt::format("{};\n{}", name, output);
   }

   class_method::class_method(node_ptr id, const source_location& location) :
      node{{}, location, counter++}
   {
      make_child(std::move(id));
   }

   auto class_method::to_string() const -> std::string
   {
      std::string name =
         fmt::format("\"{}_{}\"", magic_enum::enum_name(sem::action_type::class_method), index());
      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name,
                            magic_enum::enum_name(sem::action_type::class_method));

      return fmt::format("{};\n{}", name, output);
   }
} // namespace fr::ast
