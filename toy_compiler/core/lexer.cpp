/**
 * @file lexer.cpp
 * @brief Implement the functions defined at lexer.hpp
 * @copyright Copyright (C) 2021 wmbat.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <toy_compiler/core/lexer.hpp>

#include <toy_compiler/core/utility.hpp>

#include <range/v3/algorithm/count.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/replace.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/replace.hpp>
#include <range/v3/view/take_while.hpp>

#include <fstream>
#include <iostream>

namespace vi = ranges::views;
namespace fs = std::filesystem;

namespace lex
{
   auto lexer(const std::string_view data, std::uint32_t line) -> item;
   auto trim_leading_whitespaces(const std::string_view data)
      -> std::pair<std::string_view, std::uint32_t>;
   auto check_for_newlines(const lex::item& tok) -> std::uint32_t
   {
      if (tok.type == grammar::token_type::block_cmt)
      {
         return static_cast<std::uint32_t>(ranges::count(tok.lexeme, '\n'));
      }

      return 0;
   }
   auto cleanup_lexeme(const lex::item& tok) -> std::string
   {
      if (tok.type == grammar::token_type::block_cmt)
      {
         return to_literal(tok.lexeme);
      }

      if (tok.type == grammar::token_type::line_cmt)
      {
         return to_literal(tok.lexeme);
      }

      if (tok.type == grammar::token_type::invalid_cmt)
      {
         return to_literal(tok.lexeme);
      }

      if (tok.type == grammar::token_type::str_lit)
      {
         return tok.lexeme.substr(1, std::size(tok.lexeme) - 2);
      }

      return tok.lexeme;
   }

   auto lex_file(const fs::path& path, util::logger_wrapper log) -> monad::maybe<std::vector<item>>
   {
      std::ifstream file{path, std::ios::in};

      if (!file.is_open())
      {
         return monad::none;
      }

      log.info("tokenizing file: \"{}\"", path.c_str());

      std::vector<item> tokens;

      // TODO: look into reading file 1 kb at a time
      std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

      auto [char_view, line_counter] = trim_leading_whitespaces(content);
      line_counter += 1;

      while (std::size(char_view) != 0)
      {
         auto tok = lexer(char_view, line_counter);
         const auto [data, inc] = trim_leading_whitespaces(char_view.substr(std::size(tok.lexeme)));
         const auto extra = check_for_newlines(tok);

         tok.lexeme = cleanup_lexeme(tok);

         char_view = data;
         line_counter += inc + extra;

         tokens.push_back(tok);
      }

      log.info("tokenization of file \"{}\" completed", path.c_str());

      tokens.push_back(item{.type = grammar::token_type::eof,
                            .lexeme = "$",
                            .line = std::numeric_limits<std::uint32_t>::max()});

      return tokens;
   }

   auto lex_alphanum(const std::string_view data, std::uint32_t line) -> item
   {
      using namespace grammar::detail;

      const auto lexeme = data | vi::take_while(is_alphanum) | ranges::to<std::string>;

      if (const auto* it = ranges::find(keywords, lexeme); it != std::end(keywords))
      {
         return {.type = grammar::keyword_to_token_type(*it), .lexeme = lexeme, .line = line};
      }

      return {.type = grammar::token_type::id, .lexeme = lexeme, .line = line};
   }

   auto lex_braces(const std::string_view data, std::uint32_t line) -> item
   {
      const char lexeme = data.at(0);

      if (lexeme == grammar::open_curly)
      {
         return {.type = grammar::token_type::left_brace, .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::close_curly)
      {
         return {.type = grammar::token_type::right_brace, .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::open_square)
      {
         return {.type = grammar::token_type::left_square, .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::close_square)
      {
         return {.type = grammar::token_type::right_square, .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::open_parenth)
      {
         return {.type = grammar::token_type::left_paren, .lexeme = {lexeme}, .line = line};
      }
      if (lexeme == grammar::close_parenth)
      {
         return {.type = grammar::token_type::right_paren, .lexeme = {lexeme}, .line = line};
      }

      return {.type = grammar::token_type::invalid_char, .lexeme = {lexeme}, .line = line};
   }

   auto lex_comments(const std::string_view data, std::uint32_t line) -> item;
   auto lex_numeric(const std::string_view data, std::uint32_t line) -> item;
   auto lex_punctuation(const std::string_view data, std::uint32_t line) -> item;
   auto lex_string(const std::string_view data, std::uint32_t line) -> item;
   auto lex_operator(const std::string_view data, std::uint32_t line) -> item;

   auto lexer(const std::string_view data, std::uint32_t line) -> item
   {
      if (std::size(data) >= 2 && is_comment(data.substr(0, 2)))
      {
         return lex_comments(data, line);
      }

      const char first = data.at(0);
      if (std::isalpha(first))
      {
         return lex_alphanum(data, line);
      }

      if (is_digit(first))
      {
         return lex_numeric(data, line);
      }

      if (is_quote(first))
      {
         return lex_string(data, line);
      }

      if (is_punctuation(first))
      {
         return lex_punctuation(data, line);
      }

      if (is_braces(first))
      {
         return lex_braces(data, line);
      }

      if (is_operator(first))
      {
         return lex_operator(data, line);
      }

      return {.type = grammar::token_type::invalid_char, .lexeme = {first}, .line = line};
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

   /////////////// NUMBERS //////////////////

   auto handle_leading_zero(const std::string_view, std::uint32_t) -> item;
   auto handle_leading_nonzero(const std::string_view, std::uint32_t) -> item;
   auto handle_fraction(const std::string_view, std::uint32_t) -> item;

   auto lex_numeric(const std::string_view data, std::uint32_t line) -> item
   {
      if (data.at(0) == '0')
      {
         return handle_leading_zero(data, line);
      }

      return handle_leading_nonzero(data, line);
   }

   auto handle_leading_zero(const std::string_view data, std::uint32_t line) -> item
   {
      const char first = data.at(0);
      if (std::size(data) > 1)
      {
         const char second = data.at(1);
         if (second == grammar::period)
         {
            const auto float_token = handle_fraction(data.substr(1), line);
            return {.type = float_token.type,
                    .lexeme = first + float_token.lexeme,
                    .line = float_token.line};
         }
      }

      return {.type = grammar::token_type::integer_lit, .lexeme = {first}, .line = line};
   }
   auto handle_leading_nonzero(const std::string_view data, std::uint32_t line) -> item
   {
      const auto lexeme = data | vi::take_while(is_digit) | ranges::to<std::string>;
      const auto* const last = std::begin(data) + std::size(lexeme);

      if (last != std::end(data) && *last == grammar::period)
      {
         const auto float_token = handle_fraction(data.substr(std::size(lexeme)), line);
         return {.type = float_token.type,
                 .lexeme = lexeme + float_token.lexeme,
                 .line = float_token.line};
      }

      return {.type = grammar::token_type::integer_lit, .lexeme = lexeme, .line = line};
   }
   auto handle_scientific_notation(const std::string_view data, std::uint32_t line) -> item
   {
      const auto convert = [](const item& tok) {
         return tok.type != grammar::token_type::invalid_num ? ::grammar::token_type::float_lit
                                                             : tok.type;
      };

      const auto first = data.at(0);
      if (first == '+' || first == '-')
      {
         const auto second = data.at(1);
         if (second == '0')
         {
            const auto integer = handle_leading_zero(data.substr(1), line);
            return {.type = convert(integer), .lexeme = first + integer.lexeme, .line = line};
         }

         if (is_digit(second))
         {
            const auto integer = handle_leading_nonzero(data.substr(1), line);
            return {.type = convert(integer), .lexeme = first + integer.lexeme, .line = line};
         }

         return {.type = grammar::token_type::invalid_num, .lexeme = {first}, .line = line};
      }

      if (first == '0')
      {
         const auto integer = handle_leading_zero(data, line);
         return {.type = convert(integer), .lexeme = integer.lexeme, .line = line};
      }

      if (is_digit(first))
      {
         const auto integer = handle_leading_nonzero(data, line);
         return {.type = convert(integer), .lexeme = integer.lexeme, .line = line};
      }

      return {.type = grammar::token_type::invalid_num, .lexeme = {first}, .line = line};
   }
   auto handle_fraction_nonzero(const std::string_view data, std::uint32_t line) -> item
   {
      const auto lexeme = data | vi::take_while(is_digit) | ranges::to<std::string>;

      const auto* lexeme_end = std::begin(data) + std::size(lexeme);
      if ((lexeme_end != std::end(data)) && (*lexeme_end == 'e'))
      {
         auto scientific = handle_scientific_notation(data.substr(std::size(lexeme) + 1), line);
         scientific.lexeme = '.' + lexeme + *lexeme_end + scientific.lexeme;

         return scientific;
      }

      const auto last = std::end(lexeme) - 1;
      if (*last == '0')
      {
         return {.type = grammar::token_type::float_lit,
                 .lexeme = "." + std::string{std::begin(lexeme), last},
                 .line = line};
      }

      return {.type = grammar::token_type::float_lit, .lexeme = "." + lexeme, .line = line};
   }
   auto handle_fraction_leading_zero(const std::string_view data, std::uint32_t line) -> item
   {
      if (std::size(data) > 1) // only 0
      {
         const char second = data.at(1);
         if (second != '0')
         {
            if (is_digit(second))
            {
               return handle_fraction_nonzero(data, line);
            }

            if (second == 'e')
            {
               auto scientific = handle_scientific_notation(data.substr(2), line);
               scientific.lexeme = ".0e" + scientific.lexeme;

               return scientific;
            }
         }
      }

      return {.type = grammar::token_type::float_lit, .lexeme = ".0", .line = line};
   }
   auto handle_fraction(const std::string_view data, std::uint32_t line) -> item
   {
      const char period = *std::begin(data);
      const auto* const first = std::begin(data) + 1;

      if (std::size(data) > 1) // if nothing after period
      {
         if (*first == '0')
         {
            return handle_fraction_leading_zero(data.substr(1), line);
         }

         if (is_digit(*first))
         {
            return handle_fraction_nonzero(data.substr(1), line);
         }
      }

      return {.type = grammar::token_type::invalid_num, .lexeme = {period}, .line = line};
   }

   /////////////// COMMENTS //////////////////

   auto lex_comments(const std::string_view data, std::uint32_t line) -> item
   {
      const auto start = data.substr(0, 2);
      if (start == "/*")
      {
         const auto pos = data.find("*/");
         if (pos != std::string_view::npos)
         {
            return {.type = grammar::token_type::block_cmt,
                    .lexeme = std::string{data.substr(0, pos + 2)},
                    .line = line};
         }

         return {
            .type = grammar::token_type::invalid_cmt, .lexeme = std::string{data}, .line = line};
      }

      return {.type = grammar::token_type::line_cmt,
              .lexeme = std::string{data.substr(0, data.find_first_of('\n'))},
              .line = line};
   }

   /////////////// PUNCTUATION //////////////////

   auto handle_colon(std::string_view data, std::uint32_t line) -> item
   {
      const auto lexeme = data.substr(0, std::min(data.find_first_not_of(':'), std::size(data)));

      if (std::size(lexeme) >= 2)
      {
         // clang-format off
         return {
            .type = grammar::token_type::double_colon,
            .lexeme = {std::begin(lexeme), std::begin(lexeme) + 2}, 
            .line = line
         };
         // clang-format on
      }

      return {.type = grammar::token_type::colon, .lexeme = std::string{lexeme}, .line = line};
   }

   auto lex_punctuation(const std::string_view data, std::uint32_t line) -> item
   {
      // NOLINTNEXTLINE
      assert(is_punctuation(data.at(0)) && "first character must be a punctuation symbol");

      const char first = data.at(0);

      if (first == grammar::period)
      {
         return {.type = grammar::token_type::period, .lexeme = {first}, .line = line};
      }

      if (first == grammar::comma)
      {
         return {.type = grammar::token_type::comma, .lexeme = {first}, .line = line};
      }

      if (first == grammar::semi_colon)
      {
         return {.type = grammar::token_type::semi_colon, .lexeme = {first}, .line = line};
      }

      if (first == grammar::colon)
      {
         return handle_colon(data, line);
      }

      return {.type = grammar::token_type::invalid_char, .lexeme{first}, .line = line};
   }

   /////////// STRING TOKENIZATION //////////////

   auto handle_terminated_string(const std::string_view data, std::uint32_t line) -> item
   {
      // clang-format off
      const auto lexeme = data 
         | vi::take_while([](char c) { return is_alphanum(c) || (c == ' '); }) 
         | ranges::to<std::string>;
      // clang-format on

      if (std::size(lexeme) == std::size(data))
      {
         return {.type = grammar::token_type::str_lit,
                 .lexeme = '\"' + std::string{data} + '\"',
                 .line = line};
      }

      return {.type = grammar::token_type::invalid_str,
              .lexeme = '\"' + std::string{data} + '\"',
              .line = line};
   }

   auto lex_string(const std::string_view data, std::uint32_t line) -> item
   {
      // NOLINTNEXTLINE
      assert(data.at(0) == '\"' && "first character must be a quotation mark (\")");

      const auto next_newline = data.substr(1).find_first_of('\n');
      const auto next_quotation_mark = data.substr(1).find_first_of('\"');

      if (next_newline >= next_quotation_mark && next_quotation_mark != std::string_view::npos)
      {
         return handle_terminated_string(data.substr(1, next_quotation_mark), line);
      }

      return {.type = grammar::token_type::invalid_str,
              .lexeme = std::string{data.substr(0, next_newline + 1)},
              .line = line};
   }

   /////////// OPERATORS //////////////

   auto handle_leading_less_than(const std::string_view data, std::uint32_t line) -> item
   {
      if (std::size(data) > 1)
      {
         if (data.at(1) == '=')
         {
            return {.type = grammar::token_type::less_equal_than,
                    .lexeme = std::string{data.substr(0, 2)},
                    .line = line};
         }

         if (data.at(1) == '>')
         {
            return {.type = grammar::token_type::not_equal,
                    .lexeme = std::string{data.substr(0, 2)},
                    .line = line};
         }
      }

      return {.type = grammar::token_type::less_than, .lexeme = {data.at(0)}, .line = line};
   }
   auto handle_leading_greater_than(const std::string_view data, std::uint32_t line) -> item
   {
      if (std::size(data) > 1 && data.at(1) == '=')
      {
         return {.type = grammar::token_type::greater_equal_than,
                 .lexeme = std::string{data.substr(0, 2)},
                 .line = line};
      }

      return {.type = grammar::token_type::greater_than, .lexeme = {data.at(0)}, .line = line};
   }
   auto handle_leading_equal(const std::string_view data, std::uint32_t line) -> item
   {
      if (std::size(data) > 1 && data.at(1) == '=')
      {
         return {.type = grammar::token_type::equal,
                 .lexeme = std::string{data.substr(0, 2)},
                 .line = line};
      }

      return {.type = grammar::token_type::assign, .lexeme = {data.at(0)}, .line = line};
   }

   auto lex_operator(const std::string_view data, std::uint32_t line) -> item
   {
      const auto first = data.at(0);
      if (first == '+')
      {
         return {.type = grammar::token_type::plus, .lexeme = {first}, .line = line};
      }

      if (first == '-')
      {
         return {.type = grammar::token_type::minus, .lexeme = {first}, .line = line};
      }

      if (first == '*')
      {
         return {.type = grammar::token_type::mult, .lexeme = {first}, .line = line};
      }

      if (first == '/')
      {
         return {.type = grammar::token_type::div, .lexeme = {first}, .line = line};
      }

      if (first == '|')
      {
         return {.type = grammar::token_type::or_op, .lexeme = {first}, .line = line};
      }

      if (first == '&')
      {
         return {.type = grammar::token_type::and_op, .lexeme = {first}, .line = line};
      }

      if (first == '!')
      {
         return {.type = grammar::token_type::not_op, .lexeme = {first}, .line = line};
      }

      if (first == '?')
      {
         return {.type = grammar::token_type::qmark, .lexeme = {first}, .line = line};
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
