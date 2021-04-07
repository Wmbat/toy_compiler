#pragma once

#include <span>
#include <variant>
#include <vector>

namespace munster::ast
{
   template <typename... ChildTypes>
   class node_context
   {
   public:
      using child_type = std::variant<ChildTypes...>;

   public:
      auto children() -> std::span<const child_type>;

      void make_child(child_type&& child) { m_children.push_back(child); }
      void make_family(std::vector<child_type>&& children) { m_children = std::move(children); }

   private:
      std::vector<child_type> m_children;
   };
} // namespace munster::ast
