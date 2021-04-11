#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class member_func_decl : public decl
   {
   public:
      member_func_decl(node_ptr visibility, node_ptr location, node_ptr id,
                       node_ptr compound_params, node_ptr tail);

      [[nodiscard]] auto visibility() const -> std::string_view;
      [[nodiscard]] auto return_type() const -> std::string_view;
      [[nodiscard]] auto params() const -> std::span<const std::string>;

      [[nodiscard]] auto params_string() const -> std::string;
      [[nodiscard]] auto to_string() const -> std::string override;

      void accept(visitor_variant &visitor) const override;

   private:
      std::string m_visibility;
      std::string m_return_type;
      std::vector<std::string> m_params;
   };
} // namespace munster::ast
