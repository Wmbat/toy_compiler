#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class variable_decl : public decl
   {
   public:
      variable_decl(node_ptr type, node_ptr id, node_ptr compound_array);

      [[nodiscard]] auto type() const -> std::string_view;

      [[nodiscard]] auto to_string() const -> std::string override;

      void accept(visitor_variant& visitor) const override;

   private:
      std::string m_type;
   };

   class compound_variable_decl : public decl
   {
   public:
      compound_variable_decl(std::vector<node_ptr>&& variables);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
