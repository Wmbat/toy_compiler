#pragma once

#include <toy_compiler/munster/ast/stmt/stmt.hpp>

namespace munster::ast
{
   class func_stmt : public stmt
   {
   public:
      func_stmt(std::vector<node_ptr>&& func);

      void accept(visitor_variant& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
