#include <toy_compiler/munster/ast/decl/func_decl.hpp>

#include <toy_compiler/munster/ast/decl/func_body_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_head_decl.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   func_decl::func_decl(node_ptr function_head, node_ptr function_body)
   {
      assert(dynamic_cast<func_head_decl*>(function_head.get())); // NOLINT
      assert(dynamic_cast<func_body_decl*>(function_body.get())); // NOLINT

      make_child(std::move(function_head));
      make_child(std::move(function_body));
   }

   void func_decl::accept(visitor_variant& visitor) const
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

   auto func_decl::to_string() const -> std::string { return "func_decl"; }
} // namespace munster::ast
