#pragma once

#include <magic_enum.hpp>

#include <fmt/core.h>

#include <cstdint>

namespace front::sem
{
   enum struct action
   {
      translation_unit,
      location_decl,
      type_decl,
      id_decl,
      compound_function_decl,
      function_decl,
      function_head_decl,
      function_body_decl,
      compound_class_decl,
      class_decl,
      compound_inheritance_decl,
      inheritance_decl,
      compound_member_decl,
      member_decl,
      visibily_decl,
      variable_decl,
      compound_array_decl,
      array_decl,
      member_function_decl,
      compound_param_decl,
      compound_variable_decl,
      main_decl,
      compound_stmt,
      integer_literal,
      epsilon
   };
};

/**
 * @brief A specialization for using the `sem::action` enum in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<front::sem::action>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(front::sem::action action, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(action));
   }
};
