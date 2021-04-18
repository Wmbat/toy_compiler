#include <toy_compiler/munster/ast/decl/stmt_block_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

using namespace mpark::patterns;

namespace munster::ast
{
   stmt_block_decl::stmt_block_decl(node_ptr node)
   {
      if (node)
      {
         make_child(std::move(node));
      }
   }

   void stmt_block_decl::accept(visitor_variant& visitor) const
   {
      match(visitor)(
         pattern(as<code_gen_visitor>(arg)) =
            [](code_gen_visitor& vis) {
               vis.place_marker("stmt_block");
            },
         pattern(_) = [] {});

      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto stmt_block_decl::to_string() const -> std::string { return "stmt_block_decl"; }
} // namespace munster::ast
