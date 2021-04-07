#pragma once

#include <toy_compiler/front_end/ast/decl/decl.hpp>

namespace front::ast
{
   class func_body_decl : public decl
   {
   public:
      func_body_decl(node_ptr variables, node_ptr statements);

      void accept(visitor &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
