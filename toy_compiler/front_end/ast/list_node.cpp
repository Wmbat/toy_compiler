#include <toy_compiler/front_end/ast/list_node.hpp>

#include <range/v3/view/tail.hpp>

namespace fr::ast
{
   namespace vi = ranges::views;

   list_node::list_node(list_node_type type, std::vector<node_ptr>&& children) : m_type{type}
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

   auto list_node::to_string() const -> std::string
   {
      if (m_type == list_node_type::class_decl)
      {
         return fmt::format("{}", sem::to_string_view(sem::action_type::class_decl_list));
      }

      if (m_type == list_node_type::func_def)
      {
         return fmt::format("{}", sem::to_string_view(sem::action_type::func_def_list));
      }

      if (m_type == list_node_type::statement)
      {
         return fmt::format("{}", sem::to_string_view(sem::action_type::statement_list));
      }

      if (m_type == list_node_type::array_size)
      {
         return fmt::format("{}", sem::to_string_view(sem::action_type::array_size_list));
      }

      return fmt::format("{}", "bad");
   }
} // namespace fr::ast
