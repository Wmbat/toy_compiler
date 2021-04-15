#pragma once

#include <toy_compiler/munster/ast/decl/decl.hpp>

#include <toy_compiler/munster/source_location.hpp>

namespace munster::ast
{
   class array_index_access_decl : public decl
   {
   public:
      array_index_access_decl(node_ptr beg, node_ptr expr, node_ptr end);

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      source_location m_end;
   };

   class compound_array_index_access_decl : public decl
   {
   public:
      /**
       * @brief Construct a **Compound Node** from a list of node_ptr.
       *
       * @param[in] variables The list of nodes that should belong to the compound declaration.
       *
       * @pre elements of variables must inherit from ast::op or ast::factor_decl.
       */
      compound_array_index_access_decl(std::vector<node_ptr>&& variables);

      [[nodiscard]] auto to_string() const -> std::string override;
   };
} // namespace munster::ast
