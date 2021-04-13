#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

namespace munster::ast
{
   class array_decl : public decl
   {
   public:
      array_decl(node_ptr start_loc, node_ptr integer_lit, node_ptr end_loc);

      void accept(visitor_variant &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      source_location m_end_loc;
   };

   class compound_array_decl : public decl
   {
   public:
      compound_array_decl(std::vector<node_ptr>&& member_decl);

      void accept(visitor_variant &visitor) const override;

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
