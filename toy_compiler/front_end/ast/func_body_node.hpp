#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class func_body_node : public node
   {
   public:
      func_body_node(node_ptr method_body_var, node_ptr statement_list);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace fr::ast
