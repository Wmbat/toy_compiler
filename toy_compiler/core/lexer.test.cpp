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

#include <toy_compiler/core/lexer.hpp>

#include <fmt/ranges.h>

#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/iota.hpp>

namespace vi = ranges::views;

TEST_SUITE("Lexer test suite")
{
   TEST_CASE("non-existant file")
   {
      auto maybe = lex::lex_file("lexer/file_that_doesn't_exist.txt");

      REQUIRE(!maybe);
   }
   TEST_CASE("id test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("lexer/id_valid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 23);

         for (auto& token : std::span(std::begin(data), 22))
         {
            CHECK(token.type == grammar::token_type::id);
         }

         CHECK(data.at(22).type == grammar::token_type::eof);
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::lex_file("lexer/id_invalid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 13);
         CHECK(data.at(0).type == grammar::token_type::invalid_char);
         CHECK(data.at(0).line == 1);
         CHECK(data.at(1).type == grammar::token_type::id);
         CHECK(data.at(1).line == 1);
         CHECK(data.at(2).type == grammar::token_type::id);
         CHECK(data.at(2).line == 1);
         CHECK(data.at(3).type == grammar::token_type::id);
         CHECK(data.at(3).line == 1);
         CHECK(data.at(4).type == grammar::token_type::id);
         CHECK(data.at(4).line == 1);
         CHECK(data.at(5).type == grammar::token_type::id);
         CHECK(data.at(6).line == 1);
         CHECK(data.at(6).type == grammar::token_type::id);
         CHECK(data.at(6).line == 1);
         CHECK(data.at(7).type == grammar::token_type::invalid_char);
         CHECK(data.at(7).line == 1);
         CHECK(data.at(8).type == grammar::token_type::id);
         CHECK(data.at(8).line == 1);
         CHECK(data.at(9).type == grammar::token_type::invalid_char);
         CHECK(data.at(9).line == 1);
         CHECK(data.at(10).type == grammar::token_type::invalid_char);
         CHECK(data.at(10).line == 2);
         CHECK(data.at(11).type == grammar::token_type::id);
         CHECK(data.at(11).line == 2);
      }
   }
   TEST_CASE("integer test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("lexer/num_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         CHECK(data.at(0).type == grammar::token_type::integer_lit);
         CHECK(data.at(1).type == grammar::token_type::integer_lit);
         CHECK(data.at(2).type == grammar::token_type::integer_lit);
         CHECK(data.at(3).type == grammar::token_type::integer_lit);
         CHECK(data.at(4).type == grammar::token_type::integer_lit);
         CHECK(data.at(5).type == grammar::token_type::integer_lit);
         CHECK(data.at(6).type == grammar::token_type::integer_lit);
         CHECK(data.at(7).type == grammar::token_type::integer_lit);
         CHECK(data.at(8).type == grammar::token_type::integer_lit);
         CHECK(data.at(9).type == grammar::token_type::integer_lit);
         CHECK(data.at(10).type == grammar::token_type::integer_lit);
         CHECK(data.at(11).type == grammar::token_type::integer_lit);
         CHECK(data.at(12).type == grammar::token_type::integer_lit);
         CHECK(data.at(13).type == grammar::token_type::integer_lit);
         CHECK(data.at(14).type == grammar::token_type::integer_lit);
         CHECK(data.at(15).type == grammar::token_type::integer_lit);
         CHECK(data.at(16).type == grammar::token_type::integer_lit);
         CHECK(data.at(17).type == grammar::token_type::integer_lit);
         CHECK(data.at(18).type == grammar::token_type::integer_lit);
         CHECK(data.at(19).type == grammar::token_type::integer_lit);
         CHECK(data.at(20).type == grammar::token_type::integer_lit);
         CHECK(data.at(21).type == grammar::token_type::id);
      }
   }
   TEST_CASE("float test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("lexer/float_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : std::span(std::begin(data), std::end(data) - 1))
         {
            CHECK(tok.type == grammar::token_type::float_lit);
            CHECK(tok.line == index++);
         }
      }
      SUBCASE("weird")
      {
         using namespace lex;

         auto maybe = lex::lex_file("lexer/float_invalid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(data.at(0) == item{grammar::token_type::integer_lit, "0", 1});
         CHECK(data.at(1) == item{grammar::token_type::float_lit, "123.134", 1});
         CHECK(data.at(2) == item{grammar::token_type::integer_lit, "0", 2});
         CHECK(data.at(3) == item{grammar::token_type::integer_lit, "1", 2});
         CHECK(data.at(4) == item{grammar::token_type::id, "ab3", 2});
         CHECK(data.at(5) == item{grammar::token_type::period, ".", 2});
         CHECK(data.at(6) == item{grammar::token_type::integer_lit, "134", 2});
         CHECK(data.at(7) == item{grammar::token_type::float_lit, "0.123", 3});
         CHECK(data.at(8) == item{grammar::token_type::integer_lit, "0", 3});
         CHECK(data.at(9) == item{grammar::token_type::float_lit, "123.0123", 4});
         CHECK(data.at(10) == item{grammar::token_type::id, "ab", 4});
         CHECK(data.at(11) == item{grammar::token_type::float_lit, "123.0123", 5});
         CHECK(data.at(12) == item{grammar::token_type::id, "ab_", 5});
         CHECK(data.at(13) == item{grammar::token_type::float_lit, "0.0", 6});
         CHECK(data.at(14) == item{grammar::token_type::integer_lit, "0", 6});
         CHECK(data.at(15) == item{grammar::token_type::integer_lit, "0", 6});
         CHECK(data.at(16) == item{grammar::token_type::integer_lit, "0", 6});
         CHECK(data.at(17) == item{grammar::token_type::integer_lit, "0", 6});
         CHECK(data.at(18) == item{grammar::token_type::integer_lit, "0", 7});
         CHECK(data.at(19) == item{grammar::token_type::float_lit, "1.23", 7});
         CHECK(data.at(20) == item{grammar::token_type::integer_lit, "0", 8});
         CHECK(data.at(21) == item{grammar::token_type::float_lit, "12.34", 8});
         CHECK(data.at(22) == item{grammar::token_type::float_lit, "12.34", 9});
         CHECK(data.at(23) == item{grammar::token_type::integer_lit, "0", 9});
         CHECK(data.at(24) == item{grammar::token_type::integer_lit, "0", 10});
         CHECK(data.at(25) == item{grammar::token_type::float_lit, "12.34", 10});
         CHECK(data.at(26) == item{grammar::token_type::integer_lit, "0", 10});
         CHECK(data.at(27) == item{grammar::token_type::integer_lit, "0", 11});
         CHECK(data.at(28) == item{grammar::token_type::float_lit, "12.34e10", 11});
         CHECK(data.at(29) == item{grammar::token_type::float_lit, "12.34e0", 12});
         CHECK(data.at(30) == item{grammar::token_type::integer_lit, "10", 12});
         CHECK(data.at(31) == item{grammar::token_type::invalid_num, "12.", 13});
         CHECK(data.at(32) == item{grammar::token_type::id, "ab", 13});
         CHECK(data.at(33) == item{grammar::token_type::invalid_num, "1.2e-", 14});
         CHECK(data.at(34) == item{grammar::token_type::id, "a123", 14});
      }
   }
   TEST_CASE("string test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("lexer/string_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (auto& tok : std::span{std::begin(data), std::end(data) - 1})
         {
            CHECK(tok.type == grammar::token_type::str_lit);
         }
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::lex_file("lexer/string_invalid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         CHECK(std::size(data) == 4);

         for (auto& tok : std::span{std::begin(data), std::end(data) - 1})
         {
            CHECK(tok.type == grammar::token_type::invalid_str);
         }
      }
   }
   TEST_CASE("punctuation test")
   {
      auto maybe = lex::lex_file("lexer/punctuation_valid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            lex::item{.type = grammar::token_type::period, .lexeme = ".", .line = 1});
      CHECK(*(std::begin(data) + 1) ==
            lex::item{.type = grammar::token_type::semi_colon, .lexeme = ";", .line = 1});
      CHECK(*(std::begin(data) + 2) ==
            lex::item{.type = grammar::token_type::comma, .lexeme = ",", .line = 1});
      CHECK(*(std::begin(data) + 3) ==
            lex::item{.type = grammar::token_type::double_colon, .lexeme = "::", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            lex::item{.type = grammar::token_type::colon, .lexeme = ":", .line = 1});
   }
   TEST_CASE("operator test")
   {
      SUBCASE("valid")
      {
         using namespace lex;

         auto maybe = lex_file("lexer/operators_valid.txt");
         REQUIRE(maybe);
         const auto data = maybe.value();

         CHECK(data.at(0) == item{.type = grammar::token_type::equal, .lexeme = "==", .line = 1});
         CHECK(data.at(1) ==
               item{.type = grammar::token_type::less_equal_than, .lexeme = "<=", .line = 1});
         CHECK(data.at(2) ==
               item{.type = grammar::token_type::greater_equal_than, .lexeme = ">=", .line = 1});
         CHECK(data.at(3) ==
               item{.type = grammar::token_type::not_equal, .lexeme = "<>", .line = 1});
         CHECK(data.at(4) ==
               item{.type = grammar::token_type::greater_than, .lexeme = ">", .line = 1});
         CHECK(data.at(5) ==
               item{.type = grammar::token_type::less_than, .lexeme = "<", .line = 1});
         CHECK(data.at(6) == item{.type = grammar::token_type::plus, .lexeme = "+", .line = 3});
         CHECK(data.at(7) == item{.type = grammar::token_type::minus, .lexeme = "-", .line = 3});
         CHECK(data.at(8) == item{.type = grammar::token_type::mult, .lexeme = "*", .line = 4});
         CHECK(data.at(9) == item{.type = grammar::token_type::div, .lexeme = "/", .line = 4});
         CHECK(data.at(10) == item{.type = grammar::token_type::assign, .lexeme = "=", .line = 6});
         CHECK(data.at(11) == item{.type = grammar::token_type::or_op, .lexeme = "|", .line = 8});
         CHECK(data.at(12) == item{.type = grammar::token_type::and_op, .lexeme = "&", .line = 8});
         CHECK(data.at(13) == item{.type = grammar::token_type::not_op, .lexeme = "!", .line = 8});
         CHECK(data.at(14) == item{.type = grammar::token_type::qmark, .lexeme = "?", .line = 8});
      }
   }
   TEST_CASE("braces test")
   {
      auto maybe = lex::lex_file("lexer/braces_valid.txt");
      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            lex::item{.type = grammar::token_type::left_paren, .lexeme = "(", .line = 1});
      CHECK(*(std::begin(data) + 1) ==
            lex::item{.type = grammar::token_type::right_paren, .lexeme = ")", .line = 1});
      CHECK(*(std::begin(data) + 2) ==
            lex::item{.type = grammar::token_type::left_brace, .lexeme = "{", .line = 1});
      CHECK(*(std::begin(data) + 3) ==
            lex::item{.type = grammar::token_type::right_brace, .lexeme = "}", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            lex::item{.type = grammar::token_type::left_square, .lexeme = "[", .line = 1});
      CHECK(*(std::begin(data) + 5) ==
            lex::item{.type = grammar::token_type::right_square, .lexeme = "]", .line = 1});
   }
   TEST_CASE("comment test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("lexer/comment_valid.txt");
         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK((std::begin(data) + 0)->type == grammar::token_type::line_cmt);
         CHECK((std::begin(data) + 1)->type == grammar::token_type::block_cmt);
         CHECK((std::begin(data) + 2)->type == grammar::token_type::block_cmt);
         CHECK((std::begin(data) + 3)->type == grammar::token_type::block_cmt);
         CHECK((std::begin(data) + 3)->line == 7);
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::lex_file("lexer/comment_invalid.txt");
         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(std::begin(data)->type == grammar::token_type::invalid_cmt);
      }
   }
   TEST_CASE("invalid characters")
   {
      auto maybe = lex::lex_file("lexer/chars_invalid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            lex::item{.type = grammar::token_type::invalid_char, .lexeme = "@", .line = 1});
      CHECK(*(std::begin(data) + 1) ==
            lex::item{.type = grammar::token_type::invalid_char, .lexeme = "#", .line = 1});
      CHECK(*(std::begin(data) + 2) ==
            lex::item{.type = grammar::token_type::invalid_char, .lexeme = "$", .line = 1});
      CHECK(*(std::begin(data) + 3) ==
            lex::item{.type = grammar::token_type::invalid_char, .lexeme = "'", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            lex::item{.type = grammar::token_type::invalid_char, .lexeme = "\\", .line = 1});
      CHECK(*(std::begin(data) + 5) ==
            lex::item{.type = grammar::token_type::invalid_char, .lexeme = "~", .line = 1});
   }
   TEST_CASE("fmt::formatter - lex::item")
   {
      using namespace lex;

      item tok{.type = grammar::token_type::integer_lit, .lexeme = "hello", .line = 10};

      CHECK(fmt::format("{}", tok) ==
            fmt::format("[.type = {}, .lexeme = {}, .line = {}]", tok.type, tok.lexeme, tok.line));
   }
}
