/**
 * @file grammar_type.hpp
 * @brief Contains all element relating to the `grammar::grammar_type` enum
 * @copyright Copyright (C) 2021 wmbat.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <fmt/core.h>

#include <cstdint>
#include <functional>
#include <string_view>

namespace grammar
{
   namespace detail
   {
      /**
       * List of string representation of the `grammar::grammar_type` enumeration values
       */
      constexpr std::array grammar_names{"add_op",
                                         "a_params",
                                         "a_params_tail",
                                         "arith_expr",
                                         "arith_expr_tail",
                                         "array_size_rept",
                                         "assign_op",
                                         "assign_stat_tail",
                                         "class_decl",
                                         "class_decl_body",
                                         "class_method",
                                         "expr",
                                         "expr_tail",
                                         "factor",
                                         "f_params",
                                         "f_params_tail",
                                         "func_body",
                                         "func_decl",
                                         "func_decl_tail",
                                         "func_def",
                                         "func_head",
                                         "func_or_assign_stat",
                                         "func_or_assign_stat_idnest",
                                         "func_or_assign_stat_idnest_func_tail",
                                         "func_or_assign_stat_idnest_var_tail",
                                         "func_or_var",
                                         "func_or_var_idnest",
                                         "func_or_var_idnest_tail",
                                         "func_stat_tail",
                                         "func_stat_tail_idnest",
                                         "function",
                                         "indice_rep",
                                         "inherit",
                                         "int_num",
                                         "member_decl",
                                         "method_body_var",
                                         "mult_op",
                                         "nested_id",
                                         "prog",
                                         "relop",
                                         "sign",
                                         "start",
                                         "stat_block",
                                         "statement",
                                         "statement_list",
                                         "term",
                                         "term_tail",
                                         "type",
                                         "var_decl",
                                         "var_decl_rep",
                                         "variable",
                                         "variable_idnest",
                                         "variable_idnest_tail",
                                         "visibility",
                                         "max_size"

      };

   } // namespace detail

   enum struct grammar_type : std::uint32_t
   {
      add_op,
      a_params,
      a_params_tail,
      arith_expr,
      arith_expr_tail,
      array_size_rept,
      assign_op,
      assign_stat_tail,
      class_decl,
      class_decl_body,
      class_method,
      expr,
      expr_tail,
      factor,
      f_params,
      f_params_tail,
      func_body,
      func_decl,
      func_decl_tail,
      func_def,
      func_head,
      func_or_assign_stat,
      func_or_assign_stat_idnest,
      func_or_assign_stat_idnest_func_tail,
      func_or_assign_stat_idnest_var_tail,
      func_or_var,
      func_or_var_idnest,
      func_or_var_idnest_tail,
      func_stat_tail,
      func_stat_tail_idnest,
      function,
      indice_rep,
      inherit,
      int_num,
      member_decl,
      method_body_var,
      mult_op,
      nested_id,
      prog,
      relop,
      sign,
      start,
      stat_block,
      statement,
      statement_list,
      term,
      term_tail,
      type,
      var_decl,
      var_decl_rep,
      variable,
      variable_idnest,
      variable_idnest_tail,
      visibility,
      max_size
   };

   /**
    * @brief convert a `grammar::grammar_type` value to it's string representation
    *
    * @param [in] type The `grammar::grammar_type` value to represent as a string
    *
    * @return A `std::string_view` into the corresponding `grammar::grammar_type` string
    * representation.
    */
   constexpr auto to_string_view(grammar::grammar_type type) -> std::string_view
   {
      return detail::grammar_names.at(static_cast<std::uint32_t>(type));
   }
} // namespace grammar

/**
 * @brief A specialization for using the `grammar::grammar_type` enum in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<grammar::grammar_type>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(grammar::grammar_type type, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", grammar::to_string_view(type));
   }
};
