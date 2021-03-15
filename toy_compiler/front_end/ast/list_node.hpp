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

   class list : public node
   {
   public:
      list(list_node_type type, std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      [[nodiscard]] auto to_string_impl(sem::action_type type) const -> std::string;

   private:
      list_node_type m_type;

      std::uint32_t m_index = 0;
   };
} // namespace fr::ast
