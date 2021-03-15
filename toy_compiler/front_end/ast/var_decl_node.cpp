#include <toy_compiler/front_end/ast/var_decl_node.hpp>

#include <toy_compiler/front_end/ast/value_node.hpp>

namespace fr::ast
{
   static inline std::uint32_t counter = 0; // NOLINT

   var_decl_node::var_decl_node(node_ptr type, node_ptr value, node_ptr array_size_list) :
      m_index{counter++}
   {
      value->make_sibling(std::move(array_size_list));
      type->make_sibling(std::move(value));

      make_child(std::move(type));
   }

   auto var_decl_node::to_string() const -> std::string
   {
      std::string name =
         fmt::format("\"{}_{}\"", sem::to_string_view(sem::action_type::var_decl), m_index);
      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output +=
         fmt::format("{} [label={}]\n", name, sem::to_string_view(sem::action_type::var_decl));

      return fmt::format("{};\n{}", name, output);
   }
} // namespace fr::ast
