#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class var_decl_node : public node
   {
   public:
      var_decl_node(node_ptr type, node_ptr item, node_ptr array_size_list);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
}; // namespace fr::ast
