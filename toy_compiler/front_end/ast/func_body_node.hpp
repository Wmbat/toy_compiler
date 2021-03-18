#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class function : public node
   {
   public:
      function(node_ptr func_head, node_ptr func_body, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class func_head : public node
   {
   public:
      func_head(node_ptr id, node_ptr class_method, node_ptr function_param, node_ptr return_type,
                const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class func_body_node : public node
   {
   public:
      func_body_node(node_ptr method_body_var, node_ptr statement_list);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class class_method : public node
   {
   public:
      class_method(node_ptr id, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace fr::ast
