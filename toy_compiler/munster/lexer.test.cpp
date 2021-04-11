/**
 * @brief Runs tests on functions defined in lexer.hpp
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <toy_compiler/munster/lexer.hpp>

#include <fmt/ranges.h>

#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/iota.hpp>

namespace vi = ranges::views;

TEST_SUITE("Lexer test suite")
{
   TEST_CASE("non-existant file")
   {
      auto maybe = munster::lex_file("lexer/file_that_doesn't_exist.txt");

      REQUIRE(!maybe);
   }
   TEST_CASE("id test")
   {
      SUBCASE("valid")
      {
         auto maybe = munster::lex_file("lexer/id_valid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 23);

         for (auto& token : std::span(std::begin(data), 22))
         {
            CHECK(token.type == munster::grammar::token_type::e_id);
         }

         CHECK(data.at(22).type == munster::grammar::token_type::e_eof);
      }
      SUBCASE("invalid")
      {
         auto maybe = munster::lex_file("lexer/id_invalid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 13);
         CHECK(data.at(0).type == munster::grammar::token_type::e_invalid_char);
         CHECK(data.at(0).pos.line == 1);
         CHECK(data.at(1).type == munster::grammar::token_type::e_id);
         CHECK(data.at(1).pos.line == 1);
         CHECK(data.at(2).type == munster::grammar::token_type::e_id);
         CHECK(data.at(2).pos.line == 1);
         CHECK(data.at(3).type == munster::grammar::token_type::e_id);
         CHECK(data.at(3).pos.line == 1);
         CHECK(data.at(4).type == munster::grammar::token_type::e_id);
         CHECK(data.at(4).pos.line == 1);
         CHECK(data.at(5).type == munster::grammar::token_type::e_id);
         CHECK(data.at(6).pos.line == 1);
         CHECK(data.at(6).type == munster::grammar::token_type::e_id);
         CHECK(data.at(6).pos.line == 1);
         CHECK(data.at(7).type == munster::grammar::token_type::e_invalid_char);
         CHECK(data.at(7).pos.line == 1);
         CHECK(data.at(8).type == munster::grammar::token_type::e_id);
         CHECK(data.at(8).pos.line == 1);
         CHECK(data.at(9).type == munster::grammar::token_type::e_invalid_char);
         CHECK(data.at(9).pos.line == 1);
         CHECK(data.at(10).type == munster::grammar::token_type::e_invalid_char);
         CHECK(data.at(10).pos.line == 2);
         CHECK(data.at(11).type == munster::grammar::token_type::e_id);
         CHECK(data.at(11).pos.line == 2);
      }
   }
   TEST_CASE("integer test")
   {
      SUBCASE("valid")
      {
         auto maybe = munster::lex_file("lexer/num_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         CHECK(data.at(0).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(1).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(2).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(3).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(4).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(5).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(6).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(7).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(8).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(9).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(10).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(11).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(12).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(13).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(14).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(15).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(16).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(17).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(18).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(19).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(20).type == munster::grammar::token_type::e_integer_lit);
         CHECK(data.at(21).type == munster::grammar::token_type::e_id);
      }
   }
   TEST_CASE("float test")
   {
      SUBCASE("valid")
      {
         auto maybe = munster::lex_file("lexer/float_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : std::span(std::begin(data), std::end(data) - 1))
         {
            CHECK(tok.type == munster::grammar::token_type::e_float_lit);
            CHECK(tok.pos.line == index++);
         }
      }
      SUBCASE("weird")
      {
         auto maybe = munster::lex_file("lexer/float_invalid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         /*
         CHECK(data.at(0) == lex_item{munster::grammar::token_type::integer_lit, "0", 1});
         CHECK(data.at(1) == lex_item{munster::grammar::token_type::float_lit, "123.134", 1});
         CHECK(data.at(2) == lex_item{munster::grammar::token_type::integer_lit, "0", 2});
         CHECK(data.at(3) == lex_item{munster::grammar::token_type::integer_lit, "1", 2});
         CHECK(data.at(4) == lex_item{munster::grammar::token_type::id, "ab3", 2});
         CHECK(data.at(5) == lex_item{munster::grammar::token_type::period, ".", 2});
         CHECK(data.at(6) == lex_item{munster::grammar::token_type::integer_lit, "134", 2});
         CHECK(data.at(7) == lex_item{munster::grammar::token_type::float_lit, "0.123", 3});
         CHECK(data.at(8) == lex_item{munster::grammar::token_type::integer_lit, "0", 3});
         CHECK(data.at(9) == lex_item{munster::grammar::token_type::float_lit, "123.0123", 4});
         CHECK(data.at(10) == lex_item{munster::grammar::token_type::id, "ab", 4});
         CHECK(data.at(11) == lex_item{munster::grammar::token_type::float_lit, "123.0123", 5});
         CHECK(data.at(12) == lex_item{munster::grammar::token_type::id, "ab_", 5});
         CHECK(data.at(13) == lex_item{munster::grammar::token_type::float_lit, "0.0", 6});
         CHECK(data.at(14) == lex_item{munster::grammar::token_type::integer_lit, "0", 6});
         CHECK(data.at(15) == lex_item{munster::grammar::token_type::integer_lit, "0", 6});
         CHECK(data.at(16) == lex_item{munster::grammar::token_type::integer_lit, "0", 6});
         CHECK(data.at(17) == lex_item{munster::grammar::token_type::integer_lit, "0", 6});
         CHECK(data.at(18) == lex_item{munster::grammar::token_type::integer_lit, "0", 7});
         CHECK(data.at(19) == lex_item{munster::grammar::token_type::float_lit, "1.23", 7});
         CHECK(data.at(20) == lex_item{munster::grammar::token_type::integer_lit, "0", 8});
         CHECK(data.at(21) == lex_item{munster::grammar::token_type::float_lit, "12.34", 8});
         CHECK(data.at(22) == lex_item{munster::grammar::token_type::float_lit, "12.34", 9});
         CHECK(data.at(23) == lex_item{munster::grammar::token_type::integer_lit, "0", 9});
         CHECK(data.at(24) == lex_item{munster::grammar::token_type::integer_lit, "0", 10});
         CHECK(data.at(25) == lex_item{munster::grammar::token_type::float_lit, "12.34", 10});
         CHECK(data.at(26) == lex_item{munster::grammar::token_type::integer_lit, "0", 10});
         CHECK(data.at(27) == lex_item{munster::grammar::token_type::integer_lit, "0", 11});
         CHECK(data.at(28) == lex_item{munster::grammar::token_type::float_lit, "12.34e10", 11});
         CHECK(data.at(29) == lex_item{munster::grammar::token_type::float_lit, "12.34e0", 12});
         CHECK(data.at(30) == lex_item{munster::grammar::token_type::integer_lit, "10", 12});
         CHECK(data.at(31) == lex_item{munster::grammar::token_type::invalid_num, "12.", 13});
         CHECK(data.at(32) == lex_item{munster::grammar::token_type::id, "ab", 13});
         CHECK(data.at(33) == lex_item{munster::grammar::token_type::invalid_num, "1.2e-", 14});
         CHECK(data.at(34) == lex_item{munster::grammar::token_type::id, "a123", 14});
         */
      }
   }
   TEST_CASE("string test")
   {
      SUBCASE("valid")
      {
         auto maybe = munster::lex_file("lexer/string_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (auto& tok : std::span{std::begin(data), std::end(data) - 1})
         {
            CHECK(tok.type == munster::grammar::token_type::e_str_lit);
         }
      }
      SUBCASE("invalid")
      {
         auto maybe = munster::lex_file("lexer/string_invalid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         CHECK(std::size(data) == 4);

         for (auto& tok : std::span{std::begin(data), std::end(data) - 1})
         {
            CHECK(tok.type == munster::grammar::token_type::e_invalid_str);
         }
      }
   }
   TEST_CASE("punctuation test")
   {
      auto maybe = munster::lex_file("lexer/punctuation_valid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      /*
      CHECK(*(std::begin(data) + 0) ==
            munster::lex_item{.type = munster::grammar::token_type::period, .lexeme = ".", .line =
      1}); CHECK(*(std::begin(data) + 1) == munster::lex_item{.type =
      munster::grammar::token_type::semi_colon, .lexeme = ";", .line = 1}); CHECK(*(std::begin(data)
      + 2) == munster::lex_item{.type = munster::grammar::token_type::comma, .lexeme = ",", .line =
      1}); CHECK(*(std::begin(data) + 3)
      == munster::lex_item{.type = munster::grammar::token_type::double_colon, .lexeme = "::", .line
      = 1}); CHECK(*(std::begin(data) + 4) == munster::lex_item{.type =
      munster::grammar::token_type::colon, .lexeme = ":", .line = 1});
            */
   }
   TEST_CASE("operator test")
   {
      SUBCASE("valid")
      {
         auto maybe = munster::lex_file("lexer/operators_valid.txt");
         REQUIRE(maybe);
         const auto data = maybe.value();

         /*
         CHECK(data.at(0) == lex_item{.type = munster::grammar::token_type::equal, .lexeme = "==",
         .line = 1}); CHECK(data.at(1) == lex_item{.type =
         munster::grammar::token_type::less_equal_than, .lexeme = "<=", .line = 1});
         CHECK(data.at(2)
         == lex_item{.type = munster::grammar::token_type::greater_equal_than, .lexeme = ">=", .line
         = 1}); CHECK(data.at(3)
         == lex_item{.type = munster::grammar::token_type::not_equal, .lexeme = "<>", .line = 1});
         CHECK(data.at(4) ==
               lex_item{.type = munster::grammar::token_type::greater_than, .lexeme = ">", .line =
         1}); CHECK(data.at(5) == lex_item{.type = munster::grammar::token_type::less_than, .lexeme
         =
         "<", .line = 1}); CHECK(data.at(6) == lex_item{.type = munster::grammar::token_type::plus,
         .lexeme = "+", .line = 3}); CHECK(data.at(7) == lex_item{.type =
         munster::grammar::token_type::minus, .lexeme = "-", .line = 3}); CHECK(data.at(8) ==
         lex_item{.type = munster::grammar::token_type::mult, .lexeme =
         "*", .line = 4}); CHECK(data.at(9) == lex_item{.type = munster::grammar::token_type::div,
         .lexeme = "/", .line = 4}); CHECK(data.at(10) == lex_item{.type =
         munster::grammar::token_type::assign, .lexeme = "=", .line = 6}); CHECK(data.at(11) ==
         lex_item{.type = munster::grammar::token_type::or_op, .lexeme = "|", .line = 8});
         CHECK(data.at(12) == lex_item{.type = munster::grammar::token_type::and_op, .lexeme = "&",
         .line = 8}); CHECK(data.at(13) == lex_item{.type = munster::grammar::token_type::not_op,
         .lexeme =
         "!", .line = 8}); CHECK(data.at(14) == lex_item{.type =
         munster::grammar::token_type::qmark, .lexeme = "?", .line = 8});
         */
      }
   }
   TEST_CASE("braces test")
   {
      auto maybe = munster::lex_file("lexer/braces_valid.txt");
      REQUIRE(maybe);

      const auto data = maybe.value();

      /*
      CHECK(*(std::begin(data) + 0) ==
            munster::lex_item{.type = munster::grammar::token_type::left_paren, .lexeme = "(", .line
      = 1}); CHECK(*(std::begin(data) + 1) == munster::lex_item{.type =
      munster::grammar::token_type::right_paren, .lexeme = ")", .line = 1});
      CHECK(*(std::begin(data)
      + 2) == munster::lex_item{.type = munster::grammar::token_type::left_brace, .lexeme = "{",
      .line = 1}); CHECK(*(std::begin(data) + 3) == munster::lex_item{.type =
      munster::grammar::token_type::right_brace, .lexeme = "}", .line = 1});
      CHECK(*(std::begin(data)
      + 4) == munster::lex_item{.type = munster::grammar::token_type::left_square, .lexeme = "[",
      .line = 1}); CHECK(*(std::begin(data) + 5) == munster::lex_item{.type =
      munster::grammar::token_type::right_square, .lexeme = "]", .line = 1});
            */
   }
   TEST_CASE("comment test")
   {
      SUBCASE("valid")
      {
         auto maybe = munster::lex_file("lexer/comment_valid.txt");
         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK((std::begin(data) + 0)->type == munster::grammar::token_type::e_line_cmt);
         CHECK((std::begin(data) + 1)->type == munster::grammar::token_type::e_block_cmt);
         CHECK((std::begin(data) + 2)->type == munster::grammar::token_type::e_block_cmt);
         CHECK((std::begin(data) + 3)->type == munster::grammar::token_type::e_block_cmt);
         CHECK((std::begin(data) + 3)->pos.line == 8);
      }
      SUBCASE("invalid")
      {
         auto maybe = munster::lex_file("lexer/comment_invalid.txt");
         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(std::begin(data)->type == munster::grammar::token_type::e_invalid_cmt);
      }
   }
   TEST_CASE("invalid characters")
   {
      auto maybe = munster::lex_file("lexer/chars_invalid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            munster::lex_item{
               .type = munster::grammar::token_type::e_invalid_char, .lexeme = "@", .pos = {1, 1}});
      CHECK(*(std::begin(data) + 1) ==
            munster::lex_item{
               .type = munster::grammar::token_type::e_invalid_char, .lexeme = "#", .pos = {1, 3}});
      CHECK(*(std::begin(data) + 2) ==
            munster::lex_item{
               .type = munster::grammar::token_type::e_invalid_char, .lexeme = "$", .pos = {1, 5}});
      CHECK(*(std::begin(data) + 3) ==
            munster::lex_item{
               .type = munster::grammar::token_type::e_invalid_char, .lexeme = "'", .pos = {1, 7}});
      CHECK(*(std::begin(data) + 4) ==
            munster::lex_item{.type = munster::grammar::token_type::e_invalid_char,
                              .lexeme = "\\",
                              .pos = {1, 9}});
      CHECK(*(std::begin(data) + 5) ==
            munster::lex_item{.type = munster::grammar::token_type::e_invalid_char,
                              .lexeme = "~",
                              .pos = {1, 11}});
   }
   TEST_CASE("fmt::formatter - munster::lex_item")
   {
      munster::lex_item tok{
         .type = munster::grammar::token_type::e_integer_lit, .lexeme = "hello", .pos = {10, 10}};

      CHECK(fmt::format("{}", tok) ==
            fmt::format("[.type = {}, .lexeme = {}, .position = <line:{}, col:{}>]", tok.type,
                        tok.lexeme, tok.pos.line, tok.pos.column));
   }
}
