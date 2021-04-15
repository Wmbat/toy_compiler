#include <toy_compiler/munster/ast/decl/compound_params_decl.hpp>

#include <toy_compiler/munster/ast/decl/variable_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   compound_params_decl::compound_params_decl(std::vector<node_ptr>&& param_decls)
   {
      make_family<variable_decl>(std::move(param_decls));
   }

   void compound_params_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto compound_params_decl::to_string() const -> std::string { return "compound_param_decl"; }
} // namespace munster::ast
