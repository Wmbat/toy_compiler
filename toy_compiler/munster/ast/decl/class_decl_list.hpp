#pragma once

#include <toy_compiler/front_end/ast/visitor/element_intf.hpp>
#include <toy_compiler/munster/ast/decl/class_decl.hpp>
#include <toy_compiler/munster/ast/node_context.hpp>

namespace munster::ast
{
   class class_decl_list :
      public node_context<class_decl>,
      public front::ast::element_intf<class_decl_list>
   {
      using child_t = node_context<class_decl>::child_type;

   public:
      class_decl_list(std::vector<class_decl>&& class_decls);
   };
} // namespace munster::ast

template <>
struct fmt::formatter<munster::ast::class_decl_list> : fmt::formatter<fmt::string_view>
{
   template <typename FormatContext>
   auto format(const munster::ast::class_decl_list&, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "class_decl_list");
   }
};
