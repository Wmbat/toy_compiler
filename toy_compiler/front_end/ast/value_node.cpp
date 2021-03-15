#include <toy_compiler/front_end/ast/value_node.hpp>

namespace fr::ast
{
   value_node::value_node(std::string value, const source_location& location) :
      node(std::move(value), location)
   {}

   auto value_node::to_string() const -> std::string
   {
      return fmt::format("\"{}\"\n", lexeme().value());
   }

   integer_lit::integer_lit(std::string value, const source_location& location) :
      value_node{std::move(value), location}
   {}
} // namespace fr::ast
