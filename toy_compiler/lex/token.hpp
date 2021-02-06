#pragma once

#include <libcaramel/containers/dynamic_array.hpp>

#include <fmt/core.h>

#include <array>
#include <compare>
#include <string>
#include <string_view>

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
   string_lit,
   invalid_char,
   invalid_id,
   invalid_num,
   invalid_str,
   period,
   comma,
   colon,
   double_colon,
   semi_colon,
};

constexpr std::array token_names{
   "none",         "id",         "integer_lit",  "float_lit",       "str_lit",
   "invalid_char", "invalid_id", "invalid_num",  "invalid_str_lit", "period",
   "comma",        "colon",      "double_colon", "semi_colon"};

constexpr auto to_string(token_type type) -> std::string_view
{
   return token_names.at(static_cast<std::uint32_t>(type));
}

[[maybe_unused]] constexpr std::array keywords{
   "if",     "then",    "else", "integer",  "float", "string",  "void",
   "public", "private", "func", "var",      "class", "while",   "read",
   "write",  "return",  "main", "inherits", "break", "continue"};
[[maybe_unused]] constexpr std::array operators{"==", "<>", "<", ">", "<=", ">=", "+", "-",
                                                "*",  "/",  "=", "|", "&",  "!",  "?"};

[[maybe_unused]] constexpr std::array punctuations{grammar::period, grammar::comma, grammar::colon,
                                                   grammar::semi_colon};

struct token
{
   std::string_view tok{}; // NOLINT
   std::string lexeme{};   // NOLINT
   std::uint32_t line{};   // NOLINT

   auto operator<=>(const token& other) const -> std::strong_ordering = default;
};

template <>
struct fmt::formatter<token>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const token& tok, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "[{0}, {1}, {2}]", tok.tok, tok.lexeme, tok.line);
   }
};
