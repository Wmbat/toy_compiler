#pragma once

#include <toy_compiler/front_end/ast/decl/decl.hpp>

namespace front::ast
{
   class class_decl : public decl
   {
   public:
      using ptr = std::unique_ptr<class_decl>;

   public:
      class_decl(node_ptr class_start, node_ptr class_name, node_ptr compound_inheritance,
                 node_ptr compound_member);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
