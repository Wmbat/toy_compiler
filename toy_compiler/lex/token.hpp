/// @file token.hpp

#pragma once

#include <libcaramel/containers/dynamic_array.hpp>

#include <fmt/core.h>

#include <array>
#include <compare>
#include <string>
#include <string_view>

namespace lex
{
   namespace grammar
   {
      constexpr char period = '.';
      constexpr char comma = ',';
      constexpr char colon = ':';
      constexpr char semi_colon = ';';
      constexpr char open_parenth = '(';
      constexpr char close_parenth = ')';
      constexpr char open_square = '[';
      constexpr char close_square = ']';
      constexpr char open_curly = '{';
      constexpr char close_curly = '}';
   } // namespace grammar

   enum struct token_type : std::uint32_t
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
      add,
      sub,
      mult,
      div,
      equal,
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
    * List of all possible taken names.
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
    * @brief convert a token_type value to it's string representation
    */
   constexpr auto to_string_view(token_type type) -> std::string_view
   {
      return token_names.at(static_cast<std::uint32_t>(type));
   }

   [[maybe_unused]] constexpr std::array keywords{
      "if",     "then",    "else", "integer",  "float", "string",  "void",
      "public", "private", "func", "var",      "class", "while",   "read",
      "write",  "return",  "main", "inherits", "break", "continue"};

   /**
    * @brief Holds a the information of a parsed token.
    */
   struct token
   {
      std::string_view type{}; // NOLINT
      std::string lexeme{};    // NOLINT
      std::uint32_t line{};    // NOLINT

      auto operator<=>(const token& other) const -> std::strong_ordering = default;
   };

   /**
    * @fn is_invalid
    * @brief Check is a token is invalid using the `invalid_` prefix in `token_type`
    */
   constexpr auto is_invalid(std::string_view type_name)
   {
      return (type_name == to_string_view(token_type::invalid_char)) ||
         (type_name == to_string_view(token_type::invalid_id)) ||
         (type_name == to_string_view(token_type::invalid_num)) ||
         (type_name == to_string_view(token_type::invalid_cmt)) ||
         (type_name == to_string_view(token_type::invalid_str));
   }
} // namespace lex

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

template <>
struct fmt::formatter<lex::token_type> : fmt::formatter<std::string_view>
{
   template <typename FormatContext>
   auto format(lex::token_type t, FormatContext& ctx)
   {
      return fmt::formatter<std::string_view>::format(to_string_view(t), ctx);
   }
};

template <>
struct fmt::formatter<lex::token>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const lex::token& tok, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "[{0}, {1}, {2}]", tok.type, tok.lexeme, tok.line);
   }
};
