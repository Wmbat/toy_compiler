#pragma once

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

namespace front::ast
{
   class func_head_decl : public decl
   {
   public:
      func_head_decl(node_ptr location, node_ptr id, node_ptr class_method, node_ptr function_param,
                     node_ptr return_type);

      void accept(visitor &visitor) const override;

      [[nodiscard]] auto class_name() const -> std::optional<std::string_view>;
      [[nodiscard]] auto return_type() const -> std::string_view;
      [[nodiscard]] auto params() const -> std::span<const std::string>;
      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      std::string m_return_type;
      std::optional<std::string> m_class;
      std::vector<std::string> m_params;
   };
} // namespace front::ast
