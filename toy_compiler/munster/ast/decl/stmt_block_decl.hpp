#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class stmt_block_decl : public decl
   {
   public:
      using ptr = std::unique_ptr<stmt_block_decl>;

   public:
      stmt_block_decl(node_ptr node);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
