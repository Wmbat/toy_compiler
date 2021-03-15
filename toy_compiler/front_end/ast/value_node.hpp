#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class value_node : public node
   {
   public:
      value_node(std::string value, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class integer_lit : public value_node
   {
   public:
      integer_lit(std::string value, const source_location& location);
   };
}; // namespace fr::ast
