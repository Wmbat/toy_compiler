#include <toy_compiler/front_end/ast/array_size_node.hpp>

namespace fr::ast
{
   static inline std::uint32_t counter = 0; // NOLINT

   array_size_node::array_size_node(node_ptr inside) : m_index{counter++}
   {
      make_child(std::move(inside));
   }

   auto array_size_node::to_string() const -> std::string
   {
      std::string name = fmt::format("\"{}_{}\"", "array_size", m_index);
      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, "array_size");

      return fmt::format("{};\n{}", name, output);
   }
} // namespace fr::ast
