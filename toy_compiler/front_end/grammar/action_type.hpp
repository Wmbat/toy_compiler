#pragma once

#include <toy_compiler/util/magic_enum.hpp>

#include <fmt/core.h>

#include <cstdint>

namespace fr::sem
{
   enum struct action_type : std::uint32_t
   {
      program,
      class_decl_list,
      inheritance_decl_list,
      class_body_decl_list,
      func_def_list,
      function_param_decl_list,
      statement_list,
      var_decl_list,
      array_size_stmt_list,
      arith_expr_list,
      term_tail_expr_list,
      idnest_list,
      indice_list,
      function_decl,
      function_param_decl,
      return_type_decl,
      class_decl,
      class_body_decl,
      visibility_decl,
      member_decl,
      inherit_decl,
      function,
      func_head,
      func_body,
      class_method,
      statement,
      write_statement,
      return_statement,
      if_statement,
      while_statement,
      statement_block,
      method_body_var,
      idnest_decl,
      indice_decl,
      var_decl,
      expr,
      arith_expr,
      arith_tail_expr,
      term_expr,
      term_tail_expr,
      var_expr,
      array_size,
      value,
      sign,
      factor_list,
      factor_expr,
      type,
      add_op,
      mult_op,
      rel_op,
      epsilon
   };
} // namespace fr::sem

/**
 * @brief A specialization for using the `grammar::grammar_type` enum in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<fr::sem::action_type>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(fr::sem::action_type type, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(type));
   }
};
