#include <toy_compiler/front_end/ast/value_node.hpp>

namespace fr::ast
{
   static inline std::uint32_t value_counter = 0; // NOLINT

   value_node::value_node(std::string value, const source_location& location) :
      node(std::move(value), location, value_counter++)
   {}

   auto value_node::to_string() const -> std::string
   {
      const auto name = fmt::format("\"{}_{}\"", lexeme(), index());

      return fmt::format("{0};\n{0} [label=\"{1}\"];\n", name, lexeme());
   }

   integer_lit::integer_lit(std::string value, const source_location& location) :
      value_node{std::move(value), location}
   {}
} // namespace fr::ast
