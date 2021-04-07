#include <toy_compiler/front_end/ast/node/node.hpp>

namespace front::ast
{
   namespace vi = ranges::views;

   node::node(const source_location& location) : m_location{location} {}
   node::node(std::string lexeme, const source_location& location) :
      m_lexeme{std::move(lexeme)},
      m_location{location}
   {}

   auto node::children() const -> const std::vector<node_ptr>& { return m_children; }
   auto node::lexeme() const -> std::string_view
   {

      if (!std::empty(m_lexeme))
      {
         return std::string_view{m_lexeme};
      }

      return {};
   }
   auto node::location() const -> const source_location& { return m_location; }

   void node::make_child(node_ptr child) { m_children.push_back(std::move(child)); }
} // namespace front::ast
