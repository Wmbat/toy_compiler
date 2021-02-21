/**
 * @file utility.hpp
 * @brief Contains a set of utility functions used for the lexing of tokens.
 */

#pragma once

#include <string_view>

namespace lex
{
   /**
    * @brief Check if a character is is a letter from [a-zA-Z]
    */
   auto is_alphabet(char c) noexcept -> bool;
   /**
    * @brief Check if a character is is a letter from [a-zA-Z] or a number between [0, 9]
    */
   auto is_alphanum(char c) noexcept -> bool;
   /**
    * @brief Check if a character is is a number between [0, 9]
    */
   auto is_digit(char c) noexcept -> bool;
   auto is_quote(char c) noexcept -> bool;
   auto is_punctuation(char c) noexcept -> bool;
   auto is_operator(char c) noexcept -> bool;
   auto is_braces(char c) noexcept -> bool;
   auto is_newline(char c) noexcept -> bool;
   auto is_comment(std::string_view str) -> bool;

   auto to_literal(const std::string_view data) -> std::string;

   constexpr std::size_t kilobyte = 1024;
   constexpr auto operator""_kb(unsigned long long val) -> std::size_t { return val * kilobyte; }
} // namespace lex
