#include <toy_compiler/lex/lexer.hpp>

#include <fstream>
#include <iostream>

#include <range/v3/algorithm/find.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/take_while.hpp>

namespace vi = ranges::views;

namespace detail
{
   auto is_alphabet(char c) noexcept -> bool { return std::isalpha(c); }
   auto is_alphanum(char c) noexcept -> bool { return std::isalnum(c) || c == '_'; }
   auto is_digit(char c) noexcept -> bool { return std::isdigit(c); }
   auto is_punctuation(char c) noexcept -> bool
   {
      return ranges::find(punctuations, c) != std::end(punctuations);
   }
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
   const std::uint32_t line = line_index + 1;
   const char first = data.at(0);

   if (std::isalpha(first) || first == '_')
   {
      return create_alphanumeric_token(data, line);
   }

   if (detail::is_digit(first))
   {
      return create_digit_token(data, line_index);
   }

   if (detail::is_punctuation(first))
   {
      return create_punctuation_token(data, line);
   }

   return {.tok = to_string(token_type::invalid_char), .lexeme = {first}, .line = line};
}

auto lexer::create_alphanumeric_token(const std::string_view data, std::uint32_t line) const
   -> token
{
   const char first = data.at(0);

   if (std::isalpha(first))
   {
      return handle_valid_id(data, line);
   }

   return handle_alpha_leading_underscore(data, line);
}

auto lexer::handle_valid_id(const std::string_view data, std::uint32_t line) const -> token
{
   const auto lexeme = data | vi::take_while(detail::is_alphanum) | ranges::to<std::string>;

   if (auto* it = ranges::find(keywords, lexeme); it != std::end(keywords))
   {
      return {.tok = *it, .lexeme = lexeme, .line = line};
   }

   return {.tok = to_string(token_type::id), .lexeme = lexeme, .line = line};
}

auto lexer::handle_alpha_leading_underscore(const std::string_view data, std::uint32_t line) const
   -> token
{
   const auto lexeme = data | vi::take_while(detail::is_alphanum) | ranges::to<std::string>;

   if (std::size(lexeme) == 1)
   {
      return {.tok = to_string(token_type::invalid_char), .lexeme = lexeme, .line = line};
   }

   return {.tok = to_string(token_type::invalid_id), .lexeme = lexeme, .line = line};
}

auto lexer::create_digit_token(const std::string_view data, std::uint32_t line_index) const -> token
{
   const std::uint32_t line = line_index + 1;
   const char first = data.at(0);

   if (first == '0')
   {
      return handle_leading_zero(data, line);
   }

   return handle_leading_nonzero(data, line);
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
      return handle_continued_leading_zero(data, line);
   }

   if (second == grammar::period)
   {
      const auto float_token = handle_floats(data.substr(1), line);
      // clang-format off
      return {
         .tok = float_token.tok, 
         .lexeme = first + float_token.lexeme, 
         .line = float_token.line
      };
      // clang-format on
   }

   return {.tok = to_string(token_type::integer_lit), .lexeme = {first}, .line = line};
}
auto lexer::handle_continued_leading_zero(const std::string_view data, std::uint32_t line) const
   -> token
{
   const auto lexeme = data | vi::take_while(detail::is_alphanum) | ranges::to<std::string>;
   const auto* const last = std::begin(data) + std::size(lexeme);

   if (last != std::end(data) && *last == grammar::period)
   {
      const auto float_token = handle_floats({last, std::end(data)}, line);

      return {.tok = to_string(token_type::invalid_num),
              .lexeme = lexeme + float_token.lexeme,
              .line = float_token.line};
   }

   return {.tok = to_string(token_type::invalid_num), .lexeme = lexeme, .line = line};
}
auto lexer::handle_leading_nonzero(const std::string_view data, std::uint32_t line) const -> token
{
   const auto lexeme = data | vi::take_while(detail::is_digit) | ranges::to<std::string>;
   const auto* const last = std::begin(data) + std::size(lexeme);

   if (last == std::end(data))
   {
      return {.tok = to_string(token_type::integer_lit), .lexeme = lexeme, .line = line};
   }

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

   if (*last == grammar::period)
   {
      auto float_token = handle_floats(data.substr(std::size(lexeme)), line);

      // clang-format off
      return {
         .tok = float_token.tok, 
         .lexeme = lexeme + float_token.lexeme, 
         .line = float_token.line
      };
      // clang-format on
   }

   return {.tok = to_string(token_type::integer_lit), .lexeme = lexeme, .line = line};
}

auto lexer::handle_floats(const std::string_view data, std::uint32_t line) const -> token
{
   const char period = *std::begin(data);
   const auto* const first = std::begin(data) + 1;

   if (std::size(data) == 1) // if nothing after period
   {
      return {.tok = to_string(token_type::invalid_num), .lexeme = {period}, .line = line};
   }

   if (*first == '0')
   {
      return handle_float_zero(data.substr(1), line);
   }

   if (detail::is_digit(*first))
   {
      return handle_float_nonzero(data.substr(1), line);
   }

   return {.tok = to_string(token_type::invalid_num), .lexeme = {period}, .line = line};
}
auto lexer::handle_float_zero(const std::string_view data, std::uint32_t line) const -> token
{
   if (std::size(data) == 1) // only 0
   {
      return {.tok = to_string(token_type::float_lit), .lexeme = ".0", .line = line};
   }

   const char first = *(std::begin(data) + 1);
   if (detail::is_digit(first))
   {
      return handle_float_nonzero(data, line);
   }

   if (std::isalpha(first))
   {
      const auto lexeme = data | vi::take_while(detail::is_alphanum) | ranges::to<std::string>;

      return {.tok = to_string(token_type::invalid_num), .lexeme = "." + lexeme, .line = line};
   }

   return {.tok = to_string(token_type::float_lit), .lexeme = ".0", .line = line};
}
auto lexer::handle_float_nonzero(const std::string_view data, std::uint32_t line) const -> token
{
   const auto lexeme = data | vi::take_while(detail::is_digit) | ranges::to<std::string>;
   const auto* end = std::begin(data) + std::size(lexeme);

   if (end == std::end(data)) // end of token
   {
      const auto* last = std::begin(data) + std::size(lexeme) - 1;
      if (*last == '0')
      {
         return {.tok = to_string(token_type::invalid_num), .lexeme = "." + lexeme, .line = line};
      }

      return {.tok = to_string(token_type::float_lit), .lexeme = "." + lexeme, .line = line};
   }

   if (std::isalpha(*end))
   {
      const std::string_view leftovers = {end, std::end(data)};
      const auto extra = leftovers | vi::take_while(detail::is_alphanum) | ranges::to<std::string>;

      // clang-format off
      return {
         .tok = to_string(token_type::invalid_num), 
         .lexeme = "." + lexeme + extra, 
         .line = line
      };
      // clang-format on
   }

   return {.tok = to_string(token_type::float_lit), .lexeme = "." + lexeme, .line = line};
}

auto lexer::create_punctuation_token(const std::string_view data, std::uint32_t line) const -> token
{
   const char first = data.at(0);

   if (first == grammar::period)
   {
      return {.tok = to_string(token_type::period), .lexeme = {first}, .line = line};
   }

   if (first == grammar::comma)
   {
      return {.tok = to_string(token_type::comma), .lexeme = {first}, .line = line};
   }

   if (first == grammar::semi_colon)
   {
      return {.tok = to_string(token_type::semi_colon), .lexeme = {first}, .line = line};
   }

   if (first == grammar::colon)
   {
      return handle_colon(data, line);
   }

   return {};
}
auto lexer::handle_colon(std::string_view data, std::uint32_t line) const -> token
{
   const auto lexeme = data.substr(0, std::min(data.find_first_not_of(':'), std::size(data)));

   if (std::size(lexeme) >= 2)
   {
      // clang-format off
      return {
         .tok = to_string(token_type::double_colon), 
         .lexeme = {std::begin(lexeme), std::begin(lexeme) + 2}, 
         .line = line
      };
      // clang-format on
   }

   return {.tok = to_string(token_type::colon), .lexeme = std::string{lexeme}, .line = line};
}

auto lexer::trim_leading_whitespaces(const std::string_view data) const -> std::string_view
{
   return data.substr(std::min(data.find_first_not_of(" \t"), std::size(data)));
}
