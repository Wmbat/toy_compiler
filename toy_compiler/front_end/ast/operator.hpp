#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class op : public node
   {
   public:
      op(const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class qm_op : public op
   {
   public:
      qm_op(node_ptr cond, node_ptr expr_1, node_ptr expr_2, const source_location& location);
   };

   class not_op : public op
   {
   public:
      not_op(node_ptr factor_expr, const source_location& location);
   };

   class mult_op : public op
   {
   public:
      mult_op(node_ptr value, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class add_op : public op
   {
   public:
      add_op(node_ptr value, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class rel_op : public op
   {
   public:
      rel_op(node_ptr value, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
}; // namespace fr::ast
