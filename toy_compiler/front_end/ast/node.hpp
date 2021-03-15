#pragma once

#include <toy_compiler/front_end/grammar/action_type.hpp>
#include <toy_compiler/front_end/lexer.hpp>

#include <monads/maybe.hpp>

#include <fmt/core.h>

#include <memory>
#include <vector>

namespace fr::ast
{
   class node;

   using node_ptr = std::unique_ptr<node>;

   class node
   {
   public:
      node() = default;
      node(std::string lexeme, const source_location& location);
      node(const node& rhs) = delete;
      node(node&& rhs) = default;
      virtual ~node() = default;

      auto operator=(const node& rhs) -> node& = delete;
      auto operator=(node&& rhs) -> node& = default;

      [[nodiscard]] auto child() const -> const node_ptr&;
      [[nodiscard]] auto sibling() const -> const node_ptr&;
      [[nodiscard]] auto lexeme() const -> monad::maybe<std::string_view>;
      [[nodiscard]] auto location() const -> monad::maybe<source_location>;

      void make_sibling(node_ptr sibling);
      void make_child(node_ptr child);

      virtual auto to_string() const -> std::string = 0;

   private:
      node_ptr m_child;
      node_ptr m_sibling;

      std::string m_lexeme;
      monad::maybe<source_location> m_location;
   };

   using node_ptr = std::unique_ptr<node>;

   auto node_factory(fr::sem::action_type type, lex_item item, std::vector<node_ptr>& recs)
      -> node_ptr;
} // namespace fr::ast

template <>
struct fmt::formatter<fr::ast::node>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const fr::ast::node& node, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", node.to_string());
   }
};

template <>
struct fmt::formatter<fr::ast::node_ptr>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const fr::ast::node_ptr& node, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", node->to_string());
   }
};
