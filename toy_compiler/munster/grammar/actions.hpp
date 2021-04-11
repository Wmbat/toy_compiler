#pragma once

#include <magic_enum.hpp>

#include <fmt/core.h>

#include <cstdint>

namespace munster::grammar
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
      e_compound_class_decl,
      e_class_decl,
      e_compound_inheritance_decl,
      e_inheritance_decl,
      e_compound_member_decl,
      e_visibily_decl,
      e_variable_decl,
      e_member_var_decl,
      e_compound_array_decl,
      e_array_decl,
      e_member_func_decl,
      e_compound_param_decl,
      e_compound_variable_decl,
      e_main_decl,
      e_compound_stmt,
      e_assign_stmt,
      e_if_stmt,
      e_while_stmt,
      e_read_stmt,
      e_write_stmt,
      e_return_stmt,
      e_break_stmt,
      e_continue_stmt,
      e_func_or_assign_stmt,
      e_integer_literal,
      e_rel_op,
      e_mult_op,
      e_add_op,
      e_dot_op,
      e_assign_op,
      e_int_expr,
      e_float_expr,
      e_priority_expr,
      e_str_expr,
      e_not_expr,
      e_sign_expr,
      e_var_expr,
      e_func_expr,
      e_compound_parameter_expr_decl,
      e_compound_var_expr,
      e_ternary_expr,
      e_compound_array_index_access_decl,
      e_array_index_access_decl,
      e_stmt_block_decl,
      e_epsilon
   };
};

/**
 * @brief A specialization for using the `sem::action` enum in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<munster::grammar::action>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(munster::grammar::action action, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(action));
   }
};
