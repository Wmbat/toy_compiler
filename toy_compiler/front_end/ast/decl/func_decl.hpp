#pragma once

#include <toy_compiler/front_end/ast/decl/decl.hpp>

namespace front::ast
{
   class func_decl : public decl
   {
   public:
      using ptr = std::unique_ptr<func_decl>;

   public:
      func_decl(node_ptr function_head, node_ptr statements);

      void accept(visitor &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
