#pragma once

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

namespace front::ast
{
   class compound_func_decl : public decl
   {
   public:
      compound_func_decl(std::vector<node_ptr>&& class_decls);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
