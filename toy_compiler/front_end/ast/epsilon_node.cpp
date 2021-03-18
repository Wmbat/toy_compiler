#include <toy_compiler/front_end/ast/epsilon_node.hpp>

namespace fr::ast
{
   static inline std::uint32_t m_creation_count = 0; // NOLINT

   epsilon_node::epsilon_node() : m_index{m_creation_count++} {}

   auto epsilon_node::to_string() const -> std::string
   {
      return fmt::format("\"{0}_{1}\";\n\"{0}_{1}\" [label=epsilon];\n",
                         magic_enum::enum_name(sem::action_type::epsilon), m_index);
   }
} // namespace fr::ast
