#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class array_size_node : public node
   {
   public:
      array_size_node(lex_item item);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace fr::ast
