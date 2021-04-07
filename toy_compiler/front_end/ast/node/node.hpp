#pragma once

#include <toy_compiler/front_end/lexer.hpp>
#include <toy_compiler/front_end/sem/actions.hpp>

#include <toy_compiler/front_end/ast/visitor/visitor.hpp>

#include <toy_compiler/util/strong_type.hpp>

#include <range/v3/view/move.hpp>

#include <memory>

namespace front::ast
{
   class node;

   using node_ptr = std::unique_ptr<node>;

   namespace detail
   {
      template <typename... NodeTypes>
      auto is_type(const node_ptr& n) -> bool
      {
         return (dynamic_cast<NodeTypes*>(n.get()) || ...);
      }

   } // namespace detail

   /**
    * @pre node is not nullptr. UB otherwise
    */
   template <typename To, typename From>
   auto node_cast(std::unique_ptr<From>&& node) -> std::unique_ptr<To>
   {
      std::unique_ptr<To> result{dynamic_cast<To*>(node.release())};

      return result;
   }

   class node
   {
   public:
      node() = default;
      node(const source_location& location);
      node(std::string lexeme, const source_location& location);
      node(const node& rhs) = delete;
      node(node&& rhs) = default;
      virtual ~node() = default;

      auto operator=(const node& rhs) -> node& = delete;
      auto operator=(node&& rhs) -> node& = default;

      [[nodiscard]] auto children() const -> const std::vector<node_ptr>&;
      [[nodiscard]] auto lexeme() const -> std::string_view;
      [[nodiscard]] auto location() const -> const source_location&;

      void make_child(node_ptr child);

      virtual void accept(visitor& visitor) const = 0;

      [[nodiscard]] virtual auto to_string() const -> std::string = 0;

   protected:
      template <typename... NodeTypes>
      void make_family_e(std::vector<std::variant<std::unique_ptr<NodeTypes>...>>&& children)
      {
         for (std::variant<NodeTypes...> node : children | ranges::views::move)
         {
            make_child(std::visit(node_cast<node>, node));
         }
      }

      template <typename NodeType>
      void make_family_e(std::vector<std::unique_ptr<NodeType>>&& children)
      {
         for (std::unique_ptr<NodeType> node : children | ranges::views::move)
         {
            make_child(std::move(node));
         }
      }

      template <typename... NodeTypes>
      void make_family(std::vector<node_ptr>&& children)
      {
         for (node_ptr node : children | ranges::views::move)
         {
            assert(detail::is_type<NodeTypes...>); // NOLINT

            make_child(std::move(node));
         }
      }

   private:
      std::vector<node_ptr> m_children;

      std::string m_lexeme;
      source_location m_location;
   };
}; // namespace front::ast

template <>
struct fmt::formatter<front::ast::node>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const front::ast::node& node, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", node.to_string());
   }
};

template <>
struct fmt::formatter<front::ast::node_ptr>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const front::ast::node_ptr& node, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", node->to_string());
   }
};
