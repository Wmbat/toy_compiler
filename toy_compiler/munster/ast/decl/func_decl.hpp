#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class func_decl : public decl
   {
   public:
      using ptr = std::unique_ptr<func_decl>;

   public:
      func_decl(node_ptr function_head, node_ptr statements);

      void accept(visitor_variant &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
