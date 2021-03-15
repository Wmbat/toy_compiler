#include <toy_compiler/front_end/ast/value_node.hpp>

namespace fr::ast
{
   value_node::value_node(lex_item item) : node{std::move(item)} {}

   auto value_node::to_string() const -> std::string { return fmt::format("{}", token().lexeme); }
} // namespace fr::ast
