#pragma once

#include <toy_compiler/munster/ast/decl/class_decl_list.hpp>
#include <toy_compiler/munster/ast/decl/func_decl_list.hpp>
#include <toy_compiler/munster/ast/node_context.hpp>

#include <toy_compiler/front_end/ast/visitor/element_intf.hpp>

#include <fmt/core.h>

namespace munster::ast
{
   class translation_unit_decl :
      public node_context<class_decl_list, func_decl_list>,
      public front::ast::element_intf<translation_unit_decl>
   {
   public:
      translation_unit_decl(class_decl_list&& class_list, func_decl_list&& func_list)
      {
         make_child(class_list);
         make_child(func_list);
      }
   };
} // namespace munster::ast

template <>
struct fmt::formatter<munster::ast::translation_unit_decl> : fmt::formatter<fmt::string_view>
{
   template <typename FormatContext>
   auto format(const munster::ast::translation_unit_decl&, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "translation_unit_decl");
   }
};
