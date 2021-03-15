#include <toy_compiler/front_end/ast/epsilon_node.hpp>

namespace fr::ast
{
   auto epsilon_node::to_string() const -> std::string
   {
      return fmt::format("{}", sem::to_string_view(sem::action_type::epsilon));
   }
} // namespace fr::ast
