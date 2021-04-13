#include <toy_compiler/munster/ast/decl/compound_params_decl.hpp>

#include <toy_compiler/munster/ast/decl/variable_decl.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   compound_params_decl::compound_params_decl(std::vector<node_ptr>&& param_decls)
   {
      make_family<variable_decl>(std::move(param_decls));
   }

   void compound_params_decl::accept(visitor_variant& visitor) const
   {
      using namespace mpark::patterns;

      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      const auto visit = [this](auto& vis) {
         vis(*this);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit);
   }

   auto compound_params_decl::to_string() const -> std::string { return "compound_param_decl"; }
} // namespace munster::ast
