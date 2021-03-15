#pragma once

#include <toy_compiler/front_end/grammar/action_type.hpp>
#include <toy_compiler/front_end/lexer.hpp>

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
      node(fr::lex_item item);
      node(const node& rhs) = delete;
      node(node&& rhs) = default;
      virtual ~node() = default;

      auto operator=(const node& rhs) -> node& = delete;
      auto operator=(node&& rhs) -> node& = default;

      void make_sibling(node_ptr sibling);
      void make_child(node_ptr child);

      virtual auto to_string() const -> std::string = 0;

   protected:
      [[nodiscard]] auto child() const -> const node_ptr&;
      [[nodiscard]] auto sibling() const -> const node_ptr&;
      [[nodiscard]] auto token() const -> const fr::lex_item&;

   private:
      node_ptr m_child;
      node_ptr m_sibling;
      fr::lex_item m_item;
   };

   using node_ptr = std::unique_ptr<node>;

   auto node_factory(fr::sem::action_type type, lex_item item, std::vector<node_ptr>& recs)
      -> node_ptr;
} // namespace fr::ast

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
