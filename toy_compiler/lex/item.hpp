/**
 * @file item.hpp
 * @brief Contains all element relating to the `lex::item` struct
 */

#pragma once

#include <libcaramel/containers/dynamic_array.hpp>

#include <fmt/core.h>

#include <array>
#include <compare>
#include <string>
#include <string_view>

namespace lex
{
   /**
    * @brief Holds a the information of a parsed token.
    */
   struct item
   {
      std::string_view type{}; // NOLINT
      std::string lexeme{};    // NOLINT
      std::uint32_t line{};    // NOLINT

      auto operator<=>(const item& other) const -> std::strong_ordering = default;
   };

   /**
    * @brief All the possible item type that may be lexed from a source file
    */
   enum struct item_type : std::uint32_t
   {
      none,
      id,
      integer_lit,
      float_lit,
      str_lit,
      period,
      comma,
      colon,
      double_colon,
      semi_colon,
      add_op,
      sub_op,
      mult_op,
      div_op,
      equal_op,
      or_op,
      and_op,
      not_op,
      qmark,
      double_equal,
      less_than,
      less_equal_than,
      greater_than,
      greater_equal_than,
      not_equal,
      open_curly,
      close_curly,
      open_square,
      close_square,
      open_parenth,
      close_parenth,
      line_cmt,
      block_cmt,
      invalid_char,
      invalid_id,
      invalid_num,
      invalid_str,
      invalid_cmt
   };

   /**
    * List of string representation of the `lex::item_type` enumeration values
    */
   constexpr std::array token_names{"none",          "id",
                                    "integer_lit",   "float_lit",
                                    "str_lit",       "period",
                                    "comma",         "colon",
                                    "double_colon",  "semi_colon",
                                    "add",           "sub",
                                    "mult",          "div",
                                    "equal",         "or",
                                    "and",           "not",
                                    "qmark",         "double_equal",
                                    "less_than",     "less_equal_than",
                                    "greater_than",  "greater_equal_than",
                                    "not_equal",     "open_curly",
                                    "close_curly",   "open_square",
                                    "close_square",  "open_parenth",
                                    "close_parenth", "line_cmt",
                                    "block_cmt",     "invalid_char",
                                    "invalid_id",    "invalid_num",
                                    "invalid_str",   "invalid_cmt"};

   /**
    * @brief convert a `lex::item_type` value to it's string representation
    *
    * @param [in] type The `lex::item_type` value to represent as a string
    *
    * @return A `std::string_view` into the corresponding `lex::item_type` string representation
    * from the `lex::token_names` array
    */
   constexpr auto to_string_view(item_type type) -> std::string_view
   {
      return token_names.at(static_cast<std::uint32_t>(type));
   }

   /**
    * @brief Check if a `lex::item` is invalid using the `invalid_` prefix in `lex::item_type`
    *
    * @param [in] type_name The string representation of the `lex::item_type` to check
    *
    * @return true if `type_name` represents an invalid `lex::item_type`
    */
   constexpr auto is_invalid(std::string_view type_name) -> bool
   {
      return (type_name == to_string_view(item_type::invalid_char)) ||
         (type_name == to_string_view(item_type::invalid_id)) ||
         (type_name == to_string_view(item_type::invalid_num)) ||
         (type_name == to_string_view(item_type::invalid_cmt)) ||
         (type_name == to_string_view(item_type::invalid_str));
   }

   /**
    * @brief All possible keywords of the language
    */
   [[maybe_unused]] constexpr std::array keywords{
      "if",     "then",    "else", "integer",  "float", "string",  "void",
      "public", "private", "func", "var",      "class", "while",   "read",
      "write",  "return",  "main", "inherits", "break", "continue"};
} // namespace lex

/**
 * @brief A specialization for using `std::string_view` in the **fmt** & **spdlog** libraries
 */
template <>
struct fmt::formatter<std::string_view>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const std::string_view view, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", view);
   }
};

/**
 * @brief A specialization for using the `lex::item_type` enum in the **fmt** & **spdlog**
 * libraries
 */
template <>
struct fmt::formatter<lex::item_type> : fmt::formatter<std::string_view>
{
   template <typename FormatContext>
   auto format(lex::item_type t, FormatContext& ctx)
   {
      return fmt::formatter<std::string_view>::format(to_string_view(t), ctx);
   }
};

/**
 * @brief A specialization for using the `lex::item` struct in the **fmt** & **spdlog** libraries
 */
template <>
struct fmt::formatter<lex::item>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const lex::item& tok, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "[{0}, {1}, {2}]", tok.type, tok.lexeme, tok.line);
   }
};
