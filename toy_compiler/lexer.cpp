#include "range/v3/range/conversion.hpp"
#include <toy_compiler/lexer.hpp>

#include <fstream>
#include <iostream>

#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/remove.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/take_while.hpp>

namespace vi = ranges::views;

namespace detail
{
   auto is_alphanum(char c) noexcept -> bool
   {
      return std::isalpha(c) || std::isdigit(c) || c == '_';
   }
} // namespace detail

auto lexer::tokenize_file(const fs::path& path) -> crl::dynamic_array<token>
{
   std::ifstream file{path, std::ios::in};

   crl::dynamic_array<token> tokens;

   std::string line;
   std::uint32_t line_counter = 0;
   while (std::getline(file, line))
   {
      std::string_view line_data = trim_leading_whitespaces(line);
      while (std::size(line_data) != 0)
      {
         auto tok = next_token(line_data, line_counter);
         tokens.append(tok);

         line_data = trim_leading_whitespaces(line_data.substr(std::size(tok.lexeme)));
      }

      line_counter++;
   }

   return tokens;
}

auto lexer::next_token(const std::string_view data, std::uint32_t line_index) const -> token
{
   const char first = data.at(0);

   if (std::isalpha(first))
   {
      return handle_valid_id(data, line_index);
   }

   if (first == '_')
   {
      return handle_leading_underscore(data, line_index);
   }

   return {};
}

auto lexer::handle_valid_id(const std::string_view data, std::uint32_t line_index) const -> token
{
   const auto lexeme = data | vi::take_while(detail::is_alphanum) | ranges::to<std::string>;

   if (auto* it = ranges::find(keywords, lexeme); it != std::end(keywords))
   {
      return {.tok = *it, .lexeme = lexeme, .line = line_index + 1};
   }

   return {.tok = to_string(token_type::id), .lexeme = lexeme, .line = line_index + 1};
}

auto lexer::handle_leading_underscore(const std::string_view data, std::uint32_t line_index) const
   -> token
{
   const auto lexeme = data | vi::take_while(detail::is_alphanum) | ranges::to<std::string>;

   if (std::size(lexeme) == 1)
   {
      return {.tok = to_string(token_type::invalid_char), .lexeme = lexeme, .line = line_index + 1};
   }

   return {.tok = to_string(token_type::invalid_id), .lexeme = lexeme, .line = line_index + 1};
}

auto lexer::trim_leading_whitespaces(const std::string_view data) const -> std::string_view
{
   return data.substr(std::min(data.find_first_not_of(" \t"), std::size(data)));
}
