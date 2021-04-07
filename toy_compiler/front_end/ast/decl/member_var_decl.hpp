#pragma once

#include <toy_compiler/front_end/ast/decl/decl.hpp>

namespace front::ast
{
   class member_var_decl : public decl
   {
   public:
      member_var_decl(node_ptr visibility, node_ptr type, node_ptr id, node_ptr compound_array);

      [[nodiscard]] auto visibility() const -> std::string_view;
      [[nodiscard]] auto type() const -> std::string_view;

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      std::string m_visibility = "private";
      std::string m_type;
   };
} // namespace front::ast
