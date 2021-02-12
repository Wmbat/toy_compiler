#include <toy_compiler/lex/lexer.hpp>

#include <toy_compiler/lex/utility.hpp>

#include <range/v3/algorithm/count.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/take_while.hpp>

#include <fstream>
#include <iostream>

namespace vi = ranges::views;
namespace fs = std::filesystem;

namespace lex
{
   auto next_token(const std::string_view data, std::uint32_t line) -> token;
   auto trim_leading_whitespaces(const std::string_view data)
      -> std::pair<std::string_view, std::uint32_t>;
   auto check_for_newlines(const lex::token& tok) -> std::uint32_t
   {
      if (tok.type == to_string_view(token_type::block_cmt))
      {
         return static_cast<std::uint32_t>(ranges::count(tok.lexeme, '\n'));
      }

      return 0;
   }
   auto cleanup_lexeme(const lex::token& tok) -> std::string
   {
      if (tok.type == to_string_view(token_type::block_cmt))
      {
         return to_literal(tok.lexeme);
      }

      if (tok.type == to_string_view(token_type::line_cmt))
      {
         return to_literal(tok.lexeme);
      }

      if (tok.type == to_string_view(token_type::str_lit))
      {
         return tok.lexeme.substr(1, std::size(tok.lexeme) - 2);
      }

      return tok.lexeme;
   }

   auto tokenize_file(const fs::path& path, util::logger_wrapper log)
      -> monad::maybe<crl::dynamic_array<token>>
   {
      std::ifstream file{path, std::ios::in};

      if (!file.is_open())
      {
         return monad::none;
      }

      log.info("tokenizing file: \"{}\"", path.c_str());

      crl::dynamic_array<token> tokens;

      // TODO: look into reading file 1 kb at a time
      std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

      std::uint32_t line_counter = 1;

      const auto [data, inc] = trim_leading_whitespaces(content);
      std::string_view char_view = data;
      line_counter += inc;

      while (std::size(char_view) != 0)
      {
         auto tok = next_token(char_view, line_counter);
         const auto [data, inc] = trim_leading_whitespaces(char_view.substr(std::size(tok.lexeme)));
         const auto extra = check_for_newlines(tok);

         tok.lexeme = cleanup_lexeme(tok);

         char_view = data;
         line_counter += inc + extra;

         tokens.append(tok);
      }

      log.info("tokenization of file \"{}\" completed", path.c_str());

      return tokens;
   }

   auto next_token(const std::string_view data, std::uint32_t line) -> token
   {
      const char first = data.at(0);

      if (std::size(data) >= 2 && is_comment(data.substr(0, 2)))
      {
         return tokenize_comments(data, line);
      }

      if (std::isalpha(first))
      {
         return tokenize_alphanum(data, line);
      }

      if (is_digit(first))
      {
         return tokenize_numeric(data, line);
      }

      if (is_quote(first))
      {
         return tokenize_string(data, line);
      }

      if (is_punctuation(first))
      {
         return tokenize_punctuation(data, line);
      }

      if (is_braces(first))
      {
         return tokenize_braces(data, line);
      }

      if (is_operator(first))
      {
         return tokenize_operator(data, line);
      }

      return {.type = to_string_view(token_type::invalid_char), .lexeme = {first}, .line = line};
   }

   auto newline_counter(const std::string_view data) -> std::uint32_t
   {
      std::uint32_t newline_count = 0;
      for (char c : data)
      {
         if (is_newline(c))
         {
            ++newline_count;
         }
      }

      return newline_count;
   }
   auto trim_leading_whitespaces(const std::string_view data)
      -> std::pair<std::string_view, std::uint32_t>
   {
      const std::size_t first = data.find_first_not_of(" \n\f\r\t\v");

      return {data.substr(std::min(first, std::size(data))),
              newline_counter(data.substr(0, first))};
   }

   /////////////// ALPHANUMERIC /////////////////

   auto tokenize_alphanum(const std::string_view data, std::uint32_t line) -> token
   {
      // NOLINTNEXTLINE
      assert((std::isalpha(data.at(0)) || data.at(0) == '_') &&
             "first character must be an alphabet letter or underscore");

      const auto lexeme = data | vi::take_while(is_alphanum) | ranges::to<std::string>;

      if (const auto* it = ranges::find(keywords, lexeme); it != std::end(keywords))
      {
         return {.type = *it, .lexeme = lexeme, .line = line};
      }

      return {.type = to_string_view(token_type::id), .lexeme = lexeme, .line = line};
   }

   //////////////////////////////////////////////

   // PUNCTUATION

   auto handle_colon(std::string_view data, std::uint32_t line) -> token
   {
      const auto lexeme = data.substr(0, std::min(data.find_first_not_of(':'), std::size(data)));

      if (std::size(lexeme) >= 2)
      {
         // clang-format off
         return {
            .type = to_string_view(token_type::double_colon), 
            .lexeme = {std::begin(lexeme), std::begin(lexeme) + 2}, 
            .line = line
         };
         // clang-format on
      }

      return {
         .type = to_string_view(token_type::colon), .lexeme = std::string{lexeme}, .line = line};
   }

   auto tokenize_punctuation(const std::string_view data, std::uint32_t line) -> token
   {
      // NOLINTNEXTLINE
      assert(is_punctuation(data.at(0)) && "first character must be a punctuation symbol");

      const char first = data.at(0);

      if (first == grammar::period)
      {
         return {.type = to_string_view(token_type::period), .lexeme = {first}, .line = line};
      }

      if (first == grammar::comma)
      {
         return {.type = to_string_view(token_type::comma), .lexeme = {first}, .line = line};
      }

      if (first == grammar::semi_colon)
      {
         return {.type = to_string_view(token_type::semi_colon), .lexeme = {first}, .line = line};
      }

      if (first == grammar::colon)
      {
         return handle_colon(data, line);
      }

      return {.type = to_string_view(token_type::invalid_char), .lexeme{first}, .line = line};
   }

   // STRING TOKENIZATION

   auto handle_terminated_string(const std::string_view data, std::uint32_t line) -> token
   {
      // clang-format off
      const auto lexeme = data 
         | vi::take_while([](char c) { return is_alphanum(c) || (c == ' '); }) 
         | ranges::to<std::string>;
      // clang-format on

      if (std::size(lexeme) == std::size(data))
      {
         return {.type = to_string_view(token_type::str_lit),
                 .lexeme = '\"' + std::string{data} + '\"',
                 .line = line};
      }

      return {.type = to_string_view(token_type::invalid_str),
              .lexeme = '\"' + std::string{data} + '\"',
              .line = line};
   }

   auto tokenize_string(const std::string_view data, std::uint32_t line) -> token
   {
      // NOLINTNEXTLINE
      assert(data.at(0) == '\"' && "first character must be a quotation mark (\")");

      const auto next_newline = data.substr(1).find_first_of('\n');
      const auto next_quotation_mark = data.substr(1).find_first_of('\"');

      if (next_newline >= next_quotation_mark && next_quotation_mark != std::string_view::npos)
      {
         return handle_terminated_string(data.substr(1, next_quotation_mark), line);
      }

      return {.type = to_string_view(token_type::invalid_str),
              .lexeme = std::string{data.substr(0, next_newline + 1)},
              .line = line};
   }

   // BRACES

   auto tokenize_braces(const std::string_view data, std::uint32_t line) -> token
   {
      // NOLINTNEXTLINE
      assert(is_braces(data.at(0)) && "first character must be a brace");

      const char lexeme = data.at(0);

      if (lexeme == grammar::open_curly)
      {
         return {.type = to_string_view(token_type::open_curly), .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::close_curly)
      {
         return {.type = to_string_view(token_type::close_curly), .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::open_square)
      {
         return {.type = to_string_view(token_type::open_square), .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::close_square)
      {
         return {
            .type = to_string_view(token_type::close_square), .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::open_parenth)
      {
         return {
            .type = to_string_view(token_type::open_parenth), .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::close_parenth)
      {
         return {
            .type = to_string_view(token_type::close_parenth), .lexeme = {lexeme}, .line = line};
      }

      return {.type = to_string_view(token_type::invalid_char), .lexeme = {lexeme}, .line = line};
   }

   // COMMENTS

   auto tokenize_comments(const std::string_view data, std::uint32_t line) -> token
   {
      // NOLINTNEXTLINE
      assert(std::size(data) > 0u && "Cannot pass empty data");
      // NOLINTNEXTLINE
      assert(is_comment(data.substr(0, 2)));

      const auto start = data.substr(0, 2);
      if (start == "/*")
      {
         const auto pos = data.find("*/");
         if (pos != std::string_view::npos)
         {
            return {.type = to_string_view(token_type::block_cmt),
                    .lexeme = std::string{data.substr(0, pos + 2)},
                    .line = line};
         }

         return {.type = to_string_view(token_type::invalid_cmt),
                 .lexeme = std::string{data},
                 .line = line};
      }

      return {.type = to_string_view(token_type::line_cmt),
              .lexeme = std::string{data.substr(0, data.find_first_of('\n'))},
              .line = line};
   }

   // OPERATORS

   auto handle_leading_less_than(const std::string_view data, std::uint32_t line) -> token
   {
      if (std::size(data) > 1)
      {
         if (data.at(1) == '=')
         {
            return {.type = to_string_view(token_type::less_equal_than),
                    .lexeme = std::string{data.substr(0, 2)},
                    .line = line};
         }

         if (data.at(1) == '>')
         {
            return {.type = to_string_view(token_type::not_equal),
                    .lexeme = std::string{data.substr(0, 2)},
                    .line = line};
         }
      }

      return {.type = to_string_view(token_type::less_than), .lexeme = {data.at(0)}, .line = line};
   }
   auto handle_leading_greater_than(const std::string_view data, std::uint32_t line) -> token
   {
      if (std::size(data) > 1 && data.at(1) == '=')
      {
         return {.type = to_string_view(token_type::greater_equal_than),
                 .lexeme = std::string{data.substr(0, 2)},
                 .line = line};
      }

      return {
         .type = to_string_view(token_type::greater_than), .lexeme = {data.at(0)}, .line = line};
   }
   auto handle_leading_equal(const std::string_view data, std::uint32_t line) -> token
   {
      if (std::size(data) > 1 && data.at(1) == '=')
      {
         return {.type = to_string_view(token_type::double_equal),
                 .lexeme = std::string{data.substr(0, 2)},
                 .line = line};
      }

      return {.type = to_string_view(token_type::equal), .lexeme = {data.at(0)}, .line = line};
   }

   auto tokenize_operator(const std::string_view data, std::uint32_t line) -> token
   {
      const auto first = data.at(0);
      if (first == '+')
      {
         return {.type = to_string_view(token_type::add), .lexeme = {first}, .line = line};
      }

      if (first == '-')
      {
         return {.type = to_string_view(token_type::sub), .lexeme = {first}, .line = line};
      }

      if (first == '*')
      {
         return {.type = to_string_view(token_type::mult), .lexeme = {first}, .line = line};
      }

      if (first == '/')
      {
         return {.type = to_string_view(token_type::div), .lexeme = {first}, .line = line};
      }

      if (first == '|')
      {
         return {.type = to_string_view(token_type::or_op), .lexeme = {first}, .line = line};
      }

      if (first == '&')
      {
         return {.type = to_string_view(token_type::and_op), .lexeme = {first}, .line = line};
      }

      if (first == '!')
      {
         return {.type = to_string_view(token_type::not_op), .lexeme = {first}, .line = line};
      }

      if (first == '?')
      {
         return {.type = to_string_view(token_type::qmark), .lexeme = {first}, .line = line};
      }

      if (first == '<')
      {
         return handle_leading_less_than(data, line);
      }

      if (first == '>')
      {
         return handle_leading_greater_than(data, line);
      }

      return handle_leading_equal(data, line);
   }
} // namespace lex
