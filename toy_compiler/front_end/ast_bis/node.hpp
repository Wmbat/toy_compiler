#pragma once

#include <toy_compiler/front_end/lexer.hpp>
#include <toy_compiler/front_end/sem/actions.hpp>

#include <memory>

namespace front::ast_bis
{
   class node;

   using node_ptr = std::unique_ptr<node>;

   class node
   {
   public:
      node() = default;
      node(const fr::source_location& location);
      node(std::string lexeme, const fr::source_location& location);
      node(const node& rhs) = delete;
      node(node&& rhs) = default;
      virtual ~node() = default;

      auto operator=(const node& rhs) -> node& = delete;
      auto operator=(node&& rhs) -> node& = default;

      [[nodiscard]] auto child() const -> const node_ptr&;
      [[nodiscard]] auto sibling() const -> const node_ptr&;
      [[nodiscard]] auto lexeme() const -> std::string_view;
      [[nodiscard]] auto location() const -> const fr::source_location&;

      void make_sibling(node_ptr sibling);
      void make_child(node_ptr child);

      [[nodiscard]] virtual auto to_string() const -> std::string = 0;

   private:
      node_ptr m_child;
      node_ptr m_sibling;

      std::string m_lexeme;
      fr::source_location m_location;
   };

   auto node_factory(sem::action type, const fr::lex_item& item, std::vector<node_ptr>& recs)
      -> node_ptr;
}; // namespace front::ast_bis

template <>
struct fmt::formatter<front::ast_bis::node>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const front::ast_bis::node& node, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", node.to_string());
   }
};

template <>
struct fmt::formatter<front::ast_bis::node_ptr>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const front::ast_bis::node_ptr& node, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", node->to_string());
   }
};
