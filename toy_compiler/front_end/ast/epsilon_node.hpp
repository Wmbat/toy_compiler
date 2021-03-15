#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class epsilon_node : public node
   {
   public:
      epsilon_node() = default;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace fr::ast
