#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class list : public node
   {
   public:
      list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override = 0;

   protected:
      [[nodiscard]] auto to_string_impl(const std::string& type) const -> std::string;
   };

   class class_decl_list : public list
   {
   public:
      class_decl_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class inheritance_declaration_list : public list
   {
   public:
      inheritance_declaration_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class class_body_declaration_list : public list
   {
   public:
      class_body_declaration_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class function_list : public list
   {
   public:
      function_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class function_param_decl_list : public list
   {
   public:
      function_param_decl_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class statement_list : public list
   {
   public:
      statement_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class variable_declaration_list : public list
   {
   public:
      variable_declaration_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class array_size_statement_list : public list
   {
   public:
      array_size_statement_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class arith_expr_list : public list
   {
   public:
      arith_expr_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class term_tail_expr_list : public list
   {
   public:
      term_tail_expr_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class factor_expr_list : public list
   {
   public:
      factor_expr_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class idnest_list : public list
   {
   public:
      idnest_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class indice_list : public list
   {
   public:
      indice_list(std::vector<node_ptr>&& children);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace fr::ast
