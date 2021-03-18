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
      node(std::string lexeme, const source_location& location, std::uint32_t index);
      node(const node& rhs) = delete;
      node(node&& rhs) = default;
      virtual ~node() = default;

      auto operator=(const node& rhs) -> node& = delete;
      auto operator=(node&& rhs) -> node& = default;

      [[nodiscard]] auto child() const -> const node_ptr&;
      [[nodiscard]] auto sibling() const -> const node_ptr&;
      [[nodiscard]] auto lexeme() const -> std::string_view;
      [[nodiscard]] auto location() const -> const source_location&;
      [[nodiscard]] auto index() const -> std::uint32_t;

      void make_sibling(node_ptr sibling);
      void make_child(node_ptr child);

      [[nodiscard]] virtual auto to_string() const -> std::string = 0;

   private:
      node_ptr m_child;
      node_ptr m_sibling;

      std::string m_lexeme;
      source_location m_location;

      std::uint32_t m_index = 0;
   };

   auto node_factory(fr::sem::action_type type, const lex_item& item, std::vector<node_ptr>& recs)
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
