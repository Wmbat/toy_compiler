#pragma once

#include <magic_enum.hpp>

#include <fmt/core.h>

#include <cstdint>

namespace front::sem
{
   enum struct action
   {
      e_translation_unit,
      e_location_decl,
      e_type_decl,
      e_id_decl,
      e_dot_decl,
      e_compound_func_decl,
      e_func_decl,
      e_func_head_decl,
      e_func_body_decl,
      compound_class_decl,
      class_decl,
      compound_inheritance_decl,
      inheritance_decl,
      compound_member_decl,
      visibily_decl,
      variable_decl,
      e_member_var_decl,
      compound_array_decl,
      array_decl,
      e_member_func_decl,
      compound_param_decl,
      compound_variable_decl,
      main_decl,
      compound_stmt,
      if_stmt,
      while_stmt,
      read_stmt,
      write_stmt,
      return_stmt,
      break_stmt,
      continue_stmt,
      e_func_or_assign_stmt,
      integer_literal,
      e_rel_op,
      e_mult_op,
      e_add_op,
      e_dot_op,
      e_assign_op,
      int_expr,
      float_expr,
      priority_expr,
      str_expr,
      not_expr,
      sign_expr,
      var_expr,
      func_expr,
      compound_parameter_expr_decl,
      compound_var_expr,
      ternary_expr,
      compound_array_index_access_decl,
      array_index_access_decl,
      stmt_block_decl,
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
