#pragma once

#include <string_view>

namespace lex
{
   auto is_alphabet(char c) noexcept -> bool;
   auto is_alphanum(char c) noexcept -> bool;
   auto is_digit(char c) noexcept -> bool;
   auto is_quote(char c) noexcept -> bool;
   auto is_punctuation(char c) noexcept -> bool;
   auto is_braces(char c) noexcept -> bool;
   auto is_newline(char c) noexcept -> bool;
   auto is_comment(std::string_view str) -> bool;

   constexpr std::size_t kilobyte = 1024;
   constexpr auto operator""_kb(unsigned long long val) -> std::size_t { return val * kilobyte; }
} // namespace lex
