#include <toy_compiler/munster/ast/decl/array_access_decl.hpp>

namespace munster::ast
{
   array_index_access_decl::array_index_access_decl(node_ptr beg, node_ptr expr, node_ptr end) :
      decl{beg->location()},
      m_end{end->location()}
   {
      /*
      assert(dynamic_cast<location_decl*>(beg.get())); // NOLINT
      assert(dynamic_cast<location_decl*>(end.get())); // NOLINT
      */

      make_child(std::move(expr));
   }

   auto array_index_access_decl::to_string() const -> std::string
   {
      return fmt::format("array_index_access_decl <line:{}, col:{}> <line:{}, col:{}>",
                         location().line, location().column, m_end.line, m_end.column);
   }

   compound_array_index_access_decl::compound_array_index_access_decl(
      std::vector<node_ptr>&& variables)
   {
      make_family<array_index_access_decl>(std::move(variables));
   }

   auto compound_array_index_access_decl::to_string() const -> std::string
   {
      return "compound_array_index_access_decl";
   }
} // namespace munster::ast
