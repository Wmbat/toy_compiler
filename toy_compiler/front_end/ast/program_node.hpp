#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class program_node : public node
   {
   public:
      program_node(node_ptr class_decl_list, node_ptr func_def_list, node_ptr func_body);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace fr::ast
