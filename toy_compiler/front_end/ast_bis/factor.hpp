#pragma once

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

namespace front::ast
{
   class factor_decl : public decl
   {
   public:
      factor_decl() = default;
      factor_decl(const source_location& location);
      factor_decl(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override = 0;
   };

   class integer_factor_decl : public factor_decl
   {
   public:
      integer_factor_decl(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class float_factor_decl : public factor_decl
   {
   public:
      float_factor_decl(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class str_factor_decl : public factor_decl
   {
   public:
      str_factor_decl(const std::string& lexeme, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class expr_factor_decl : public factor_decl
   {
   public:
      expr_factor_decl(node_ptr location, node_ptr expr);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class not_factor_decl : public factor_decl
   {
   public:
      not_factor_decl(node_ptr value, node_ptr factor);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

   class sign_factor_decl : public factor_decl
   {
   public:
      sign_factor_decl(node_ptr sign, node_ptr factor);

      [[nodiscard]] auto to_string() const -> std::string override;
   };

} // namespace front::ast
