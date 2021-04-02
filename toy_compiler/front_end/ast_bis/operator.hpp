#pragma once

#include <toy_compiler/front_end/ast_bis/node.hpp>

namespace front::ast
{
   class op : public node
   {
   public:
      op() = default;
      op(const source_location& location);
      op(const std::string& lexeme, const source_location& location);

      void accept(visitor &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class add_op : public op
   {
   public:
      add_op(node_ptr term_0, node_ptr value, node_ptr term_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class mult_op : public op
   {
   public:
      mult_op(node_ptr factor_0, node_ptr value, node_ptr factor_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class rel_op : public op
   {
   public:
      rel_op(node_ptr arith_expr_0, node_ptr value, node_ptr arith_expr_1);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class assign_op : public node
   {
   };
} // namespace front::ast
