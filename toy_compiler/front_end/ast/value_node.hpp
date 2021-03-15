#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class value_node : public node
   {
   public:
      value_node(lex_item item);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
}; // namespace fr::ast
