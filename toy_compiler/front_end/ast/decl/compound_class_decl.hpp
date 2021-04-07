#pragma once

#include <toy_compiler/front_end/ast/decl/decl.hpp>

#include <toy_compiler/front_end/ast/decl/class_decl.hpp>

namespace front::ast
{
   class compound_class_decl : public decl
   {
   public:
      using ptr = std::unique_ptr<compound_class_decl>;

   public:
      compound_class_decl(std::vector<class_decl::ptr>&& class_decls);

      void accept(visitor& visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace front::ast
