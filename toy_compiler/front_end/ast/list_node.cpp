#include <toy_compiler/front_end/ast/list_node.hpp>

#include <range/v3/view/tail.hpp>

namespace fr::ast
{
   namespace vi = ranges::views;

   static inline std::uint32_t counter = 0;

   list::list(list_node_type type, std::vector<node_ptr>&& children) :
      m_type{type},
      m_index{counter++}
   {
      node* it = children.front().get();
      for (node_ptr& node : children | vi::tail)
      {
         auto temp = node.get();
         it->make_sibling(std::move(node));
         it = temp;
      }

      make_child(std::move(children.front()));
   }

   auto list::to_string() const -> std::string
   {
      if (m_type == list_node_type::class_decl)
      {
         return to_string_impl(sem::action_type::class_decl_list);
      }

      if (m_type == list_node_type::func_def)
      {
         return to_string_impl(sem::action_type::func_def_list);
      }

      if (m_type == list_node_type::var_decl)
      {
         return to_string_impl(sem::action_type::var_decl_list);
      }

      if (m_type == list_node_type::statement)
      {
         return to_string_impl(sem::action_type::statement_list);
      }

      if (m_type == list_node_type::array_size)
      {
         return to_string_impl(sem::action_type::array_size_list);
      }

      return fmt::format("{}", "bad");
   }

   auto list::to_string_impl(sem::action_type type) const -> std::string
   {
      std::string name = fmt::format("\"{}_{}\"", sem::to_string_view(type), m_index);

      std::string output = name + fmt::format(" -> {}", child());
      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, sem::to_string_view(type));

      return fmt::format("{0};\n{1}", name, output);
   }
} // namespace fr::ast
