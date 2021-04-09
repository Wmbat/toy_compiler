#include <toy_compiler/munster/ast/decl/func_body_decl.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   func_body_decl::func_body_decl(node_ptr variables, node_ptr statements)
   {
      if (!std::empty(variables->children()))
      {
         make_child(std::move(variables));
      }

      if (!std::empty(statements->children()))
      {
         make_child(std::move(statements));
      }
   }

   void func_body_decl::accept(visitor_variant& visitor) const
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

   auto func_body_decl::to_string() const -> std::string { return "func_body_decl"; }
} // namespace munster::ast
