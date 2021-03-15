#include <toy_compiler/front_end/ast/array_size_node.hpp>

namespace fr::ast
{
   array_size_node::array_size_node(lex_item item) : node(std::move(item)) {}

   auto array_size_node::to_string() const -> std::string
   {
      return fmt::format("{}({})", sem::to_string_view(sem::action_type::array_size),
                         token().lexeme);
   }
} // namespace fr::ast
