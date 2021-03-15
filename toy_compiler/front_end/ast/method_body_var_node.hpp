#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class method_body_var_node : public node
   {
   public:
      method_body_var_node(node_ptr var_decl_list);

      auto to_string() const -> std::string override;
   };
} // namespace fr::ast
