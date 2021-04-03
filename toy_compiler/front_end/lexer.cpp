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

#include <toy_compiler/front_end/lexer.hpp>

#include <toy_compiler/front_end/utility.hpp>

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

/**
 * @brief All possible keywords of the language
 */
constexpr std::array keywords{"if",     "then",    "else", "integer",  "float", "string",  "void",
                              "public", "private", "func", "var",      "class", "while",   "read",
                              "write",  "return",  "main", "inherits", "break", "continue"};

namespace front
{
   auto lexer(const std::string_view data, source_location pos) -> lex_item;
   auto trim_leading_whitespaces(const std::string_view data)
      -> std::tuple<std::string_view, std::uint32_t, std::uint32_t>;
   auto check_for_newlines(const lex_item& tok) -> std::uint32_t
   {
      if (tok.type == sem::token_type::e_line_cmt)
      {
         return 1;
      }

      if (tok.type == sem::token_type::e_block_cmt)
      {
         return static_cast<std::uint32_t>(ranges::count(tok.lexeme, '\n'));
      }

      return 0;
   }
   auto cleanup_lexeme(const lex_item& tok) -> std::string
   {
      if (tok.type == sem::token_type::e_block_cmt)
      {
         return to_literal(tok.lexeme);
      }

      if (tok.type == sem::token_type::e_line_cmt)
      {
         return to_literal(tok.lexeme);
      }

      if (tok.type == sem::token_type::e_invalid_cmt)
      {
         return to_literal(tok.lexeme);
      }

      if (tok.type == sem::token_type::e_str_lit)
      {
         return tok.lexeme.substr(1, std::size(tok.lexeme) - 2);
      }

      return tok.lexeme;
   }

   auto lex_file(const fs::path& path, util::logger_wrapper log)
      -> monad::maybe<std::vector<lex_item>>
   {
      std::ifstream file{path, std::ios::in};

      if (!file.is_open())
      {
         return monad::none;
      }

      log.info("tokenizing file: \"{}\"", path.c_str());

      std::vector<lex_item> tokens;

      // TODO: look into reading file 1 kb at a time
      std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

      auto [char_view, line_counter, column_counter] = trim_leading_whitespaces(content);
      line_counter += 1;
      column_counter += 1;

      while (std::size(char_view) != 0)
      {
         auto tok = lexer(char_view, {.line = line_counter, .column = column_counter});
         const auto [data, inc, col] =
            trim_leading_whitespaces(char_view.substr(std::size(tok.lexeme)));
         const auto extra = check_for_newlines(tok);

         char_view = data;
         line_counter += inc + extra;

         if (inc > 0)
         {
            column_counter = col + static_cast<std::uint32_t>(std::size(tok.lexeme));
         }
         else
         {
            column_counter += col + static_cast<std::uint32_t>(std::size(tok.lexeme));
         }

         if (extra > 0)
         {
            column_counter = 0;
         }

         tok.lexeme = cleanup_lexeme(tok);

         tokens.push_back(tok);
      }

      log.info("tokenization of file \"{}\" completed", path.c_str());

      tokens.push_back(lex_item{.type = sem::token_type::e_eof, .lexeme = "$"});

      return tokens;
   }

   auto lex_alphanum(const std::string_view data, source_location pos) -> lex_item
   {
      const auto lexeme = data | vi::take_while(is_alphanum) | ranges::to<std::string>;

      if (const auto* it = ranges::find(keywords, lexeme); it != std::end(keywords))
      {
         std::string adjusted = "e_";
         adjusted.append(*it);
         return {.type = magic_enum::enum_cast<sem::token_type>(adjusted).value(),
                 .lexeme = lexeme,
                 .pos = pos};
      }

      return {.type = sem::token_type::e_id, .lexeme = lexeme, .pos = pos};
   }

   auto lex_braces(const std::string_view data, source_location pos) -> lex_item
   {
      const char lexeme = data.at(0);

      if (lexeme == sem::open_curly)
      {
         return {.type = sem::token_type::e_left_brace, .lexeme = {lexeme}, .pos = pos};
      }
      if (lexeme == sem::close_curly)
      {
         return {.type = sem::token_type::e_right_brace, .lexeme = {lexeme}, .pos = pos};
      }
      if (lexeme == sem::open_square)
      {
         return {.type = sem::token_type::e_left_square, .lexeme = {lexeme}, .pos = pos};
      }
      if (lexeme == sem::close_square)
      {
         return {.type = sem::token_type::e_right_square, .lexeme = {lexeme}, .pos = pos};
      }
      if (lexeme == sem::open_parenth)
      {
         return {.type = sem::token_type::e_left_paren, .lexeme = {lexeme}, .pos = pos};
      }
      if (lexeme == sem::close_parenth)
      {
         return {.type = sem::token_type::e_right_paren, .lexeme = {lexeme}, .pos = pos};
      }

      return {.type = sem::token_type::e_invalid_char, .lexeme = {lexeme}, .pos = pos};
   }

   auto lex_comments(const std::string_view data, source_location pos) -> lex_item;
   auto lex_numeric(const std::string_view data, source_location pos) -> lex_item;
   auto lex_punctuation(const std::string_view data, source_location pos) -> lex_item;
   auto lex_string(const std::string_view data, source_location pos) -> lex_item;
   auto lex_operator(const std::string_view data, source_location pos) -> lex_item;

   auto lexer(const std::string_view data, source_location pos) -> lex_item
   {
      if (std::size(data) >= 2 && is_comment(data.substr(0, 2)))
      {
         return lex_comments(data, pos);
      }

      const char first = data.at(0);
      if (std::isalpha(first))
      {
         return lex_alphanum(data, pos);
      }

      if (is_digit(first))
      {
         return lex_numeric(data, pos);
      }

      if (is_quote(first))
      {
         return lex_string(data, pos);
      }

      if (is_punctuation(first))
      {
         return lex_punctuation(data, pos);
      }

      if (is_braces(first))
      {
         return lex_braces(data, pos);
      }

      if (is_operator(first))
      {
         return lex_operator(data, pos);
      }

      return {.type = sem::token_type::e_invalid_char, .lexeme = {first}, .pos = pos};
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
   auto column_counter(const std::string_view data) -> std::uint32_t
   {
      std::uint32_t column_index = 0;
      for (char c : data)
      {
         if (is_newline(c))
         {
            column_index = 0;
         }
         else
         {
            ++column_index;
         }
      }

      return column_index;
   }
   auto trim_leading_whitespaces(const std::string_view data)
      -> std::tuple<std::string_view, std::uint32_t, std::uint32_t>
   {
      const std::size_t first = data.find_first_not_of(" \n\f\r\t\v");

      return {data.substr(std::min(first, std::size(data))), newline_counter(data.substr(0, first)),
              column_counter(data.substr(0, first))};
   }

   /////////////// NUMBERS //////////////////

   auto handle_leading_zero(const std::string_view, source_location) -> lex_item;
   auto handle_leading_nonzero(const std::string_view, source_location) -> lex_item;
   auto handle_fraction(const std::string_view, source_location) -> lex_item;

   auto lex_numeric(const std::string_view data, source_location pos) -> lex_item
   {
      if (data.at(0) == '0')
      {
         return handle_leading_zero(data, pos);
      }

      return handle_leading_nonzero(data, pos);
   }

   auto handle_leading_zero(const std::string_view data, source_location pos) -> lex_item
   {
      const char first = data.at(0);
      if (std::size(data) > 1)
      {
         const char second = data.at(1);
         if (second == sem::period)
         {
            const auto float_token = handle_fraction(data.substr(1), pos);
            return {.type = float_token.type,
                    .lexeme = first + float_token.lexeme,
                    .pos = float_token.pos};
         }
      }

      return {.type = sem::token_type::e_integer_lit, .lexeme = {first}, .pos = pos};
   }
   auto handle_leading_nonzero(const std::string_view data, source_location pos) -> lex_item
   {
      const auto lexeme = data | vi::take_while(is_digit) | ranges::to<std::string>;
      const auto* const last = std::begin(data) + std::size(lexeme);

      if (last != std::end(data) && *last == sem::period)
      {
         const auto float_token = handle_fraction(data.substr(std::size(lexeme)), pos);
         return {.type = float_token.type,
                 .lexeme = lexeme + float_token.lexeme,
                 .pos = float_token.pos};
      }

      return {.type = sem::token_type::e_integer_lit, .lexeme = lexeme, .pos = pos};
   }
   auto handle_scientific_notation(const std::string_view data, source_location pos) -> lex_item
   {
      const auto convert = [](const lex_item& tok) {
         return tok.type != sem::token_type::e_invalid_num ? sem::token_type::e_float_lit
                                                           : tok.type;
      };

      const auto first = data.at(0);
      if (first == '+' || first == '-')
      {
         const auto second = data.at(1);
         if (second == '0')
         {
            const auto integer = handle_leading_zero(data.substr(1), pos);
            return {.type = convert(integer), .lexeme = first + integer.lexeme, .pos = pos};
         }

         if (is_digit(second))
         {
            const auto integer = handle_leading_nonzero(data.substr(1), pos);
            return {.type = convert(integer), .lexeme = first + integer.lexeme, .pos = pos};
         }

         return {.type = sem::token_type::e_invalid_num, .lexeme = {first}, .pos = pos};
      }

      if (first == '0')
      {
         const auto integer = handle_leading_zero(data, pos);
         return {.type = convert(integer), .lexeme = integer.lexeme, .pos = pos};
      }

      if (is_digit(first))
      {
         const auto integer = handle_leading_nonzero(data, pos);
         return {.type = convert(integer), .lexeme = integer.lexeme, .pos = pos};
      }

      return {.type = sem::token_type::e_invalid_num, .lexeme = {first}, .pos = pos};
   }
   auto handle_fraction_nonzero(const std::string_view data, source_location pos) -> lex_item
   {
      const auto lexeme = data | vi::take_while(is_digit) | ranges::to<std::string>;

      const auto* lexeme_end = std::begin(data) + std::size(lexeme);
      if ((lexeme_end != std::end(data)) && (*lexeme_end == 'e'))
      {
         auto scientific = handle_scientific_notation(data.substr(std::size(lexeme) + 1), pos);
         scientific.lexeme = '.' + lexeme + *lexeme_end + scientific.lexeme;

         return scientific;
      }

      const auto last = std::end(lexeme) - 1;
      if (*last == '0')
      {
         return {.type = sem::token_type::e_float_lit,
                 .lexeme = "." + std::string{std::begin(lexeme), last},
                 .pos = pos};
      }

      return {.type = sem::token_type::e_float_lit, .lexeme = "." + lexeme, .pos = pos};
   }
   auto handle_fraction_leading_zero(const std::string_view data, source_location pos) -> lex_item
   {
      if (std::size(data) > 1) // only 0
      {
         const char second = data.at(1);
         if (second != '0')
         {
            if (is_digit(second))
            {
               return handle_fraction_nonzero(data, pos);
            }

            if (second == 'e')
            {
               auto scientific = handle_scientific_notation(data.substr(2), pos);
               scientific.lexeme = ".0e" + scientific.lexeme;

               return scientific;
            }
         }
      }

      return {.type = sem::token_type::e_float_lit, .lexeme = ".0", .pos = pos};
   }
   auto handle_fraction(const std::string_view data, source_location pos) -> lex_item
   {
      const char period = *std::begin(data);
      const auto* const first = std::begin(data) + 1;

      if (std::size(data) > 1) // if nothing after period
      {
         if (*first == '0')
         {
            return handle_fraction_leading_zero(data.substr(1), pos);
         }

         if (is_digit(*first))
         {
            return handle_fraction_nonzero(data.substr(1), pos);
         }
      }

      return {.type = sem::token_type::e_invalid_num, .lexeme = {period}, .pos = pos};
   }

   /////////////// COMMENTS //////////////////

   auto lex_comments(const std::string_view data, source_location pos) -> lex_item
   {
      const auto start = data.substr(0, 2);
      if (start == "/*")
      {
         const auto str_pos = data.find("*/");
         if (str_pos != std::string_view::npos)
         {
            return {.type = sem::token_type::e_block_cmt,
                    .lexeme = std::string{data.substr(0, str_pos + 2)},
                    .pos = pos};
         }

         return {.type = sem::token_type::e_invalid_cmt, .lexeme = std::string{data}, .pos = pos};
      }

      return {.type = sem::token_type::e_line_cmt,
              .lexeme = std::string{data.substr(0, data.find_first_of('\n'))},
              .pos = pos};
   }

   /////////////// PUNCTUATION //////////////////

   auto handle_colon(std::string_view data, source_location pos) -> lex_item
   {
      const auto lexeme = data.substr(0, std::min(data.find_first_not_of(':'), std::size(data)));

      if (std::size(lexeme) >= 2)
      {
         return {.type = sem::token_type::e_double_colon,
                 .lexeme = {std::begin(lexeme), std::begin(lexeme) + 2},
                 .pos = pos};
      }

      return {.type = sem::token_type::e_colon, .lexeme = std::string{lexeme}, .pos = pos};
   }

   auto lex_punctuation(const std::string_view data, source_location pos) -> lex_item
   {
      // NOLINTNEXTLINE
      assert(is_punctuation(data.at(0)) && "first character must be a punctuation symbol");

      const char first = data.at(0);

      if (first == sem::period)
      {
         return {.type = sem::token_type::e_dot, .lexeme = {first}, .pos = pos};
      }

      if (first == sem::comma)
      {
         return {.type = sem::token_type::e_comma, .lexeme = {first}, .pos = pos};
      }

      if (first == sem::semi_colon)
      {
         return {.type = sem::token_type::e_semi_colon, .lexeme = {first}, .pos = pos};
      }

      if (first == sem::colon)
      {
         return handle_colon(data, pos);
      }

      return {.type = sem::token_type::e_invalid_char, .lexeme{first}, .pos = pos};
   }

   /////////// STRING TOKENIZATION //////////////

   auto handle_terminated_string(const std::string_view data, source_location pos) -> lex_item
   {
      // clang-format off
      const auto lexeme = data 
         | vi::take_while([](char c) { return is_alphanum(c) || (c == ' '); }) 
         | ranges::to<std::string>;
      // clang-format on

      if (std::size(lexeme) == std::size(data))
      {
         return {.type = sem::token_type::e_str_lit,
                 .lexeme = '\"' + std::string{data} + '\"',
                 .pos = pos};
      }

      return {.type = sem::token_type::e_invalid_str,
              .lexeme = '\"' + std::string{data} + '\"',
              .pos = pos};
   }

   auto lex_string(const std::string_view data, source_location pos) -> lex_item
   {
      // NOLINTNEXTLINE
      assert(data.at(0) == '\"' && "first character must be a quotation mark (\")");

      const auto next_newline = data.substr(1).find_first_of('\n');
      const auto next_quotation_mark = data.substr(1).find_first_of('\"');

      if (next_newline >= next_quotation_mark && next_quotation_mark != std::string_view::npos)
      {
         return handle_terminated_string(data.substr(1, next_quotation_mark), pos);
      }

      return {.type = sem::token_type::e_invalid_str,
              .lexeme = std::string{data.substr(0, next_newline + 1)},
              .pos = pos};
   }

   /////////// OPERATORS //////////////

   auto handle_leading_less_than(const std::string_view data, source_location pos) -> lex_item
   {
      if (std::size(data) > 1)
      {
         if (data.at(1) == '=')
         {
            return {.type = sem::token_type::e_less_equal_than,
                    .lexeme = std::string{data.substr(0, 2)},
                    .pos = pos};
         }

         if (data.at(1) == '>')
         {
            return {.type = sem::token_type::e_not_equal,
                    .lexeme = std::string{data.substr(0, 2)},
                    .pos = pos};
         }
      }

      return {.type = sem::token_type::e_less_than, .lexeme = {data.at(0)}, .pos = pos};
   }
   auto handle_leading_greater_than(const std::string_view data, source_location pos) -> lex_item
   {
      if (std::size(data) > 1 && data.at(1) == '=')
      {
         return {.type = sem::token_type::e_greater_equal_than,
                 .lexeme = std::string{data.substr(0, 2)},
                 .pos = pos};
      }

      return {.type = sem::token_type::e_greater_thane, .lexeme = {data.at(0)}, .pos = pos};
   }
   auto handle_leading_equal(const std::string_view data, source_location pos) -> lex_item
   {
      if (std::size(data) > 1 && data.at(1) == '=')
      {
         return {
            .type = sem::token_type::e_equal, .lexeme = std::string{data.substr(0, 2)}, .pos = pos};
      }

      return {.type = sem::token_type::e_assign, .lexeme = {data.at(0)}, .pos = pos};
   }

   auto lex_operator(const std::string_view data, source_location pos) -> lex_item
   {
      const auto first = data.at(0);
      if (first == '+')
      {
         return {.type = sem::token_type::e_plus, .lexeme = {first}, .pos = pos};
      }

      if (first == '-')
      {
         return {.type = sem::token_type::e_minus, .lexeme = {first}, .pos = pos};
      }

      if (first == '*')
      {
         return {.type = sem::token_type::e_mult, .lexeme = {first}, .pos = pos};
      }

      if (first == '/')
      {
         return {.type = sem::token_type::e_div, .lexeme = {first}, .pos = pos};
      }

      if (first == '|')
      {
         return {.type = sem::token_type::e_or, .lexeme = {first}, .pos = pos};
      }

      if (first == '&')
      {
         return {.type = sem::token_type::e_and, .lexeme = {first}, .pos = pos};
      }

      if (first == '!')
      {
         return {.type = sem::token_type::e_not, .lexeme = {first}, .pos = pos};
      }

      if (first == '?')
      {
         return {.type = sem::token_type::e_qmark, .lexeme = {first}, .pos = pos};
      }

      if (first == '<')
      {
         return handle_leading_less_than(data, pos);
      }

      if (first == '>')
      {
         return handle_leading_greater_than(data, pos);
      }

      return handle_leading_equal(data, pos);
   }
} // namespace front
