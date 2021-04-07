#pragma once

#include <toy_compiler/front_end/ast/stmt/stmt.hpp>

namespace front::ast
{
   class func_or_assign_stmt : public stmt
   {
   public:
      func_or_assign_stmt(std::vector<node_ptr>&& var_or_func_decls);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
