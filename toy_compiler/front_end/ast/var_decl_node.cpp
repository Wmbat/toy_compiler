#include <toy_compiler/front_end/ast/var_decl_node.hpp>

#include <toy_compiler/front_end/ast/value_node.hpp>

namespace fr::ast
{
   var_decl_node::var_decl_node(node_ptr type, node_ptr value, node_ptr array_size_list)
   {
      value->make_sibling(std::move(array_size_list));
      type->make_sibling(std::move(value));

      make_child(std::move(type));
   }

   auto var_decl_node::to_string() const -> std::string
   {
      return "";
      /*
      return fmt::format("{}({}, {}, {})", sem::to_string_view(sem::action_type::var_decl),
                         get_token().lexeme, );
                         */
   }
} // namespace fr::ast
