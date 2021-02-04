#include <toy_compiler/lex/lexer.hpp>

#include <fstream>
#include <iostream>

#include <range/v3/algorithm/find.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/take_while.hpp>

namespace vi = ranges::views;

namespace detail
{
   auto is_alphanum(char c) noexcept -> bool
   {
      return std::isalpha(c) || std::isdigit(c) || c == '_';
   }
   auto is_digit(char c) noexcept -> bool { return std::isdigit(c); }
} // namespace detail

lexer::lexer(util::logger_wrapper log) : m_log{log} {}

auto lexer::tokenize_file(const fs::path& path) -> monad::maybe<crl::dynamic_array<token>>
{
   std::ifstream file{path, std::ios::in};

   if (!file.is_open())
   {
      return monad::none;
   }

   m_log.info("tokenizing file: {}", path.c_str());

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

   m_log.info("tokenization of file {} completed", path.c_str());

   return tokens;
}

auto lexer::next_token(const std::string_view data, std::uint32_t line_index) const -> token
{
   const char first = data.at(0);

   if (std::isalpha(first) || first == '_')
   {
      return create_alphanumeric_lexeme(data, line_index);
   }

   if (detail::is_digit(first))
   {
      return create_digit_lexeme(data, line_index);
   }

   return {};
}

auto lexer::create_alphanumeric_lexeme(const std::string_view data, std::uint32_t line_index) const
   -> token
{
   const char first = data.at(0);

   if (std::isalpha(first))
   {
      return handle_valid_id(data, line_index);
   }

   return handle_leading_underscore(data, line_index);
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

auto lexer::create_digit_lexeme(const std::string_view data, std::uint32_t line_index) const
   -> token
{
   const std::uint32_t line = line_index + 1;
   const char first = data.at(0);

   if (first == '0')
   {
      return handle_leading_zero(data, line);
   }

   return handle_regular_digit(data, line);
}

auto lexer::handle_leading_zero(const std::string_view data, std::uint32_t line) const -> token
{
   const char first = data.at(0);
   if (std::size(data) == 1)
   {
      return {.tok = to_string(token_type::integer_lit), .lexeme = {first}, .line = line};
   }

   const char second = data.at(1);
   if (detail::is_alphanum(second))
   {
      const auto lexeme = data | vi::take_while(detail::is_alphanum) | ranges::to<std::string>;
      return {.tok = to_string(token_type::invalid_num), .lexeme = lexeme, .line = line};
   }

   return {.tok = to_string(token_type::integer_lit), .lexeme = {first}, .line = line};
}

auto lexer::handle_regular_digit(const std::string_view data, std::uint32_t line) const -> token
{
   const auto lexeme = data | vi::take_while(detail::is_digit) | ranges::to<std::string>;
   const auto* const last = std::begin(data) + std::size(lexeme);

   if (detail::is_alphanum(*last))
   {
      // clang-format off
      const auto error_lex = data 
         | vi::take_while([](char c) { return detail::is_digit(c) || detail::is_alphanum(c); }) 
         | ranges::to<std::string>;
      // clang-format on

      return {.tok = to_string(token_type::invalid_num), .lexeme = error_lex, .line = line};
      // handle error
   }

   return {.tok = to_string(token_type::integer_lit), .lexeme = lexeme, .line = line};
}

auto lexer::trim_leading_whitespaces(const std::string_view data) const -> std::string_view
{
   return data.substr(std::min(data.find_first_not_of(" \t"), std::size(data)));
}
