#pragma once

#include <fmt/core.h>

#include <cstdint>

namespace fr::sem
{
   namespace detail
   {
      constexpr std::array action_names{
         "program",         "class_decl_list", "func_def_list",   "statement_list", "var_decl_list",
         "array_size_list", "func_body",       "method_body_var", "var_decl",       "array_size",
         "factor",          "factor_value",    "epsilon"};
   }

   enum struct action_type : std::uint32_t
   {
      program,
      class_decl_list,
      func_def_list,
      statement_list,
      var_decl_list,
      array_size_list,
      func_body,
      method_body_var,
      var_decl,
      array_size,
      factor,
      factor_value,
      epsilon
   };

   /**
    * @brief convert a `sem::action_type` value to it's string representation
    *
    * @param [in] type The `sem::action_type` value to represent as a string
    *
    * @return A `std::string_view` into the corresponding `sem::action_type` string
    * representation.
    */
   constexpr auto to_string_view(sem::action_type type) -> std::string_view
   {
      return detail::action_names.at(static_cast<std::uint32_t>(type));
   }

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
      return fmt::format_to(ctx.out(), "{}", fr::sem::to_string_view(type));
   }
};
