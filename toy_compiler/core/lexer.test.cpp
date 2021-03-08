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
      auto maybe = lex::lex_file("test_files/file_that_doesn't_exist.txt");

      REQUIRE(!maybe);
   }
   TEST_CASE("id test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("test_files/id_valid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 22);

         for (auto& token : data)
         {
            CHECK(token.type == grammar::token_type::id);
         }
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::lex_file("test_files/id_invalid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 12);
         CHECK(data.lookup(0).type == grammar::token_type::invalid_char);
         CHECK(data.lookup(0).line == 1);
         CHECK(data.lookup(1).type == grammar::token_type::id);
         CHECK(data.lookup(1).line == 1);
         CHECK(data.lookup(2).type == grammar::token_type::id);
         CHECK(data.lookup(2).line == 1);
         CHECK(data.lookup(3).type == grammar::token_type::id);
         CHECK(data.lookup(3).line == 1);
         CHECK(data.lookup(4).type == grammar::token_type::id);
         CHECK(data.lookup(4).line == 1);
         CHECK(data.lookup(5).type == grammar::token_type::id);
         CHECK(data.lookup(6).line == 1);
         CHECK(data.lookup(6).type == grammar::token_type::id);
         CHECK(data.lookup(6).line == 1);
         CHECK(data.lookup(7).type == grammar::token_type::invalid_char);
         CHECK(data.lookup(7).line == 1);
         CHECK(data.lookup(8).type == grammar::token_type::id);
         CHECK(data.lookup(8).line == 1);
         CHECK(data.lookup(9).type == grammar::token_type::invalid_char);
         CHECK(data.lookup(9).line == 1);
         CHECK(data.lookup(10).type == grammar::token_type::invalid_char);
         CHECK(data.lookup(10).line == 2);
         CHECK(data.lookup(11).type == grammar::token_type::id);
         CHECK(data.lookup(11).line == 2);
      }
   }
   TEST_CASE("integer test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("test_files/num_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         CHECK(data.lookup(0).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(1).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(2).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(3).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(4).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(5).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(6).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(7).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(8).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(9).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(10).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(11).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(12).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(13).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(14).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(15).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(16).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(17).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(18).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(19).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(20).type == grammar::token_type::integer_lit);
         CHECK(data.lookup(21).type == grammar::token_type::id);
      }
   }
   TEST_CASE("float test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("test_files/float_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : data)
         {
            CHECK(tok.type == grammar::token_type::float_lit);
            CHECK(tok.line == index++);
         }
      }
      SUBCASE("weird")
      {
         using namespace lex;

         auto maybe = lex::lex_file("test_files/float_invalid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(data.lookup(0) == item{grammar::token_type::integer_lit, "0", 1});
         CHECK(data.lookup(1) == item{grammar::token_type::float_lit, "123.134", 1});
         CHECK(data.lookup(2) == item{grammar::token_type::integer_lit, "0", 2});
         CHECK(data.lookup(3) == item{grammar::token_type::integer_lit, "1", 2});
         CHECK(data.lookup(4) == item{grammar::token_type::id, "ab3", 2});
         CHECK(data.lookup(5) == item{grammar::token_type::period, ".", 2});
         CHECK(data.lookup(6) == item{grammar::token_type::integer_lit, "134", 2});
         CHECK(data.lookup(7) == item{grammar::token_type::float_lit, "0.123", 3});
         CHECK(data.lookup(8) == item{grammar::token_type::integer_lit, "0", 3});
         CHECK(data.lookup(9) == item{grammar::token_type::float_lit, "123.0123", 4});
         CHECK(data.lookup(10) == item{grammar::token_type::id, "ab", 4});
         CHECK(data.lookup(11) == item{grammar::token_type::float_lit, "123.0123", 5});
         CHECK(data.lookup(12) == item{grammar::token_type::id, "ab_", 5});
         CHECK(data.lookup(13) == item{grammar::token_type::float_lit, "0.0", 6});
         CHECK(data.lookup(14) == item{grammar::token_type::integer_lit, "0", 6});
         CHECK(data.lookup(15) == item{grammar::token_type::integer_lit, "0", 6});
         CHECK(data.lookup(16) == item{grammar::token_type::integer_lit, "0", 6});
         CHECK(data.lookup(17) == item{grammar::token_type::integer_lit, "0", 6});
         CHECK(data.lookup(18) == item{grammar::token_type::integer_lit, "0", 7});
         CHECK(data.lookup(19) == item{grammar::token_type::float_lit, "1.23", 7});
         CHECK(data.lookup(20) == item{grammar::token_type::integer_lit, "0", 8});
         CHECK(data.lookup(21) == item{grammar::token_type::float_lit, "12.34", 8});
         CHECK(data.lookup(22) == item{grammar::token_type::float_lit, "12.34", 9});
         CHECK(data.lookup(23) == item{grammar::token_type::integer_lit, "0", 9});
         CHECK(data.lookup(24) == item{grammar::token_type::integer_lit, "0", 10});
         CHECK(data.lookup(25) == item{grammar::token_type::float_lit, "12.34", 10});
         CHECK(data.lookup(26) == item{grammar::token_type::integer_lit, "0", 10});
         CHECK(data.lookup(27) == item{grammar::token_type::integer_lit, "0", 11});
         CHECK(data.lookup(28) == item{grammar::token_type::float_lit, "12.34e10", 11});
         CHECK(data.lookup(29) == item{grammar::token_type::float_lit, "12.34e0", 12});
         CHECK(data.lookup(30) == item{grammar::token_type::integer_lit, "10", 12});
         CHECK(data.lookup(31) == item{grammar::token_type::invalid_num, "12.", 13});
         CHECK(data.lookup(32) == item{grammar::token_type::id, "ab", 13});
         CHECK(data.lookup(33) == item{grammar::token_type::invalid_num, "1.2e-", 14});
         CHECK(data.lookup(34) == item{grammar::token_type::id, "a123", 14});
      }
   }
   TEST_CASE("string test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("test_files/string_valid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         for (auto& tok : data)
         {
            CHECK(tok.type == grammar::token_type::str_lit);
         }
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::lex_file("test_files/string_invalid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(std::size(data) == 3);

         for (auto& tok : data)
         {
            CHECK(tok.type == grammar::token_type::invalid_str);
         }
      }
   }
   TEST_CASE("punctuation test")
   {
      auto maybe = lex::lex_file("test_files/punctuation_valid.txt");

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

         auto maybe = lex_file("test_files/operators_valid.txt");
         REQUIRE(maybe);
         const auto data = maybe.value();

         CHECK(data.lookup(0) ==
               item{.type = grammar::token_type::equal, .lexeme = "==", .line = 1});
         CHECK(data.lookup(1) ==
               item{.type = grammar::token_type::less_equal_than, .lexeme = "<=", .line = 1});
         CHECK(data.lookup(2) ==
               item{.type = grammar::token_type::greater_equal_than, .lexeme = ">=", .line = 1});
         CHECK(data.lookup(3) ==
               item{.type = grammar::token_type::not_equal, .lexeme = "<>", .line = 1});
         CHECK(data.lookup(4) ==
               item{.type = grammar::token_type::greater_than, .lexeme = ">", .line = 1});
         CHECK(data.lookup(5) ==
               item{.type = grammar::token_type::less_than, .lexeme = "<", .line = 1});
         CHECK(data.lookup(6) ==
               item{.type = grammar::token_type::add_op, .lexeme = "+", .line = 3});
         CHECK(data.lookup(7) ==
               item{.type = grammar::token_type::sub_op, .lexeme = "-", .line = 3});
         CHECK(data.lookup(8) ==
               item{.type = grammar::token_type::mult_op, .lexeme = "*", .line = 4});
         CHECK(data.lookup(9) ==
               item{.type = grammar::token_type::div_op, .lexeme = "/", .line = 4});
         CHECK(data.lookup(10) ==
               item{.type = grammar::token_type::assign, .lexeme = "=", .line = 6});
         CHECK(data.lookup(11) ==
               item{.type = grammar::token_type::or_op, .lexeme = "|", .line = 8});
         CHECK(data.lookup(12) ==
               item{.type = grammar::token_type::and_op, .lexeme = "&", .line = 8});
         CHECK(data.lookup(13) ==
               item{.type = grammar::token_type::not_op, .lexeme = "!", .line = 8});
         CHECK(data.lookup(14) ==
               item{.type = grammar::token_type::qmark, .lexeme = "?", .line = 8});
      }
   }
   TEST_CASE("braces test")
   {
      auto maybe = lex::lex_file("test_files/braces_valid.txt");
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
         auto maybe = lex::lex_file("test_files/comment_valid.txt");
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
         auto maybe = lex::lex_file("test_files/comment_invalid.txt");
         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(std::begin(data)->type == grammar::token_type::invalid_cmt);
      }
   }
   TEST_CASE("invalid characters")
   {
      auto maybe = lex::lex_file("test_files/chars_invalid.txt");

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

      CHECK(fmt::format("{}", tok) == fmt::format("[{}, {}, {}]", tok.type, tok.lexeme, tok.line));
   }
}
