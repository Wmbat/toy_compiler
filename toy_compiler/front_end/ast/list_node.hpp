#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   enum struct list_node_type
   {
      class_decl,
      func_def,
      statement,
      var_decl,
      array_size
   };

   class list_node : public node
   {
   public:
      list_node(list_node_type type, std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      list_node_type m_type;
   };
} // namespace fr::ast
