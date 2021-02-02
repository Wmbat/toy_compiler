#pragma once

#include <array>
#include <compare>
#include <string>
#include <string_view>

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
   invalid_str
};

constexpr std::array token_names{"none",       "id",          "integer_lit",
                                 "float_lit",  "string_lit",  "invalid_char",
                                 "invalid_id", "invalid_num", "invalid_str"};

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
[[maybe_unused]] constexpr std::array punctuations{"(", ")", "{", "}", "[", "]",
                                                   ";", ",", ".", ":", "::"};

struct token
{
   std::string_view tok{}; // NOLINT
   std::string lexeme{};   // NOLINT
   std::uint32_t line{};   // NOLINT

   auto operator<=>(const token& other) const -> std::strong_ordering = default;
};
