#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class expr : public node
   {
   public:
      expr(const source_location& location);
      expr(node_ptr arith_expr, const source_location& location);
      expr(node_ptr arith_expr_0, node_ptr rel_op, node_ptr arith_expr_1,
           const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;

   protected:
      [[nodiscard]] auto to_string_impl(sem::action_type type) const -> std::string;
   };

   class arith_expr : public expr
   {
   public:
      arith_expr(node_ptr term, node_ptr arith_tail_expr_list, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class arith_tail_expr : public expr
   {
   public:
      arith_tail_expr(node_ptr add_op, node_ptr term, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class term_expr : public expr
   {
   public:
      term_expr(node_ptr factor, node_ptr term_tail_expr_list, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class term_tail_expr : public expr
   {
   public:
      term_tail_expr(node_ptr mult_op, node_ptr factor, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class factor_expr : public expr
   {
   public:
      factor_expr(const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class float_lit_factor_expr : public expr
   {
   public:
      float_lit_factor_expr(node_ptr value, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class integer_lit_factor_expr : public expr
   {
   public:
      integer_lit_factor_expr(node_ptr value, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class string_lit_factor_expr : public expr
   {
   public:
      string_lit_factor_expr(node_ptr value, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class variable_expr : public expr
   {
   public:
      variable_expr(node_ptr id, node_ptr idnest_list, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
}; // namespace fr::ast
