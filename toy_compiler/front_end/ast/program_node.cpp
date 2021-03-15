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
      return fmt::format("{}", sem::to_string_view(sem::action_type::program));
   }
} // namespace fr::ast
