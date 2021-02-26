#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <toy_compiler/lex/lexer.hpp>

#include <fmt/ranges.h>

#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/iota.hpp>

namespace vi = ranges::views;

/*
auto main(int argc, char** argv) -> int
{
   doctest::Context context;
   context.applyCommandLine(argc, argv);
   context.setOption("no-breaks", true);

   int res = context.run(); // run

   if (context.shouldExit())
      return res;

   return res;
}
*/

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
            CHECK(token.type == to_string_view(lex::item_type::id));
         }
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::lex_file("test_files/id_invalid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 12);
         CHECK(data.lookup(0).type == to_string_view(lex::item_type::invalid_char));
         CHECK(data.lookup(0).line == 1);
         CHECK(data.lookup(1).type == to_string_view(lex::item_type::id));
         CHECK(data.lookup(1).line == 1);
         CHECK(data.lookup(2).type == to_string_view(lex::item_type::id));
         CHECK(data.lookup(2).line == 1);
         CHECK(data.lookup(3).type == to_string_view(lex::item_type::id));
         CHECK(data.lookup(3).line == 1);
         CHECK(data.lookup(4).type == to_string_view(lex::item_type::id));
         CHECK(data.lookup(4).line == 1);
         CHECK(data.lookup(5).type == to_string_view(lex::item_type::id));
         CHECK(data.lookup(6).line == 1);
         CHECK(data.lookup(6).type == to_string_view(lex::item_type::id));
         CHECK(data.lookup(6).line == 1);
         CHECK(data.lookup(7).type == to_string_view(lex::item_type::invalid_char));
         CHECK(data.lookup(7).line == 1);
         CHECK(data.lookup(8).type == to_string_view(lex::item_type::id));
         CHECK(data.lookup(8).line == 1);
         CHECK(data.lookup(9).type == to_string_view(lex::item_type::invalid_char));
         CHECK(data.lookup(9).line == 1);
         CHECK(data.lookup(10).type == to_string_view(lex::item_type::invalid_char));
         CHECK(data.lookup(10).line == 2);
         CHECK(data.lookup(11).type == to_string_view(lex::item_type::id));
         CHECK(data.lookup(11).line == 2);
      }
   }
   TEST_CASE("keyword test")
   {
      SUBCASE("positive lex::keywords")
      {
         auto maybe = lex::lex_file("test_files/keyword_valid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         for (std::size_t line_counter = 0; auto i : vi::iota(0u, std::size(lex::keywords)))
         {
            if (i % 7 == 0) // NOLINT
            {
               line_counter++;
            }

            CHECK(data.lookup(i).type == lex::keywords.at(i));
            CHECK(data.lookup(i).lexeme == lex::keywords.at(i));
            CHECK(data.lookup(i).line == line_counter);
         }
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::lex_file("test_files/keyword_invalid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 12);

         CHECK(data.lookup(0).type == "if");
         CHECK(data.lookup(0).lexeme == "if");
         CHECK(data.lookup(0).line == 1);

         CHECK(data.lookup(1).type == "then");
         CHECK(data.lookup(1).lexeme == "then");
         CHECK(data.lookup(1).line == 1);

         CHECK(data.lookup(2).type == "while");
         CHECK(data.lookup(2).lexeme == "while");
         CHECK(data.lookup(2).line == 1);

         CHECK(data.lookup(3).type == "id");
         CHECK(data.lookup(3).lexeme == "ifthen");
         CHECK(data.lookup(3).line == 1);

         CHECK(data.lookup(4).type == "id");
         CHECK(data.lookup(4).lexeme == "megatest");
         CHECK(data.lookup(4).line == 1);

         CHECK(data.lookup(5).type == "integer");
         CHECK(data.lookup(5).lexeme == "integer");
         CHECK(data.lookup(5).line == 1);

         CHECK(data.lookup(6).type == "float");
         CHECK(data.lookup(6).lexeme == "float");
         CHECK(data.lookup(6).line == 2);

         CHECK(data.lookup(7).type == "string");
         CHECK(data.lookup(7).lexeme == "string");
         CHECK(data.lookup(7).line == 2);

         CHECK(data.lookup(8).type == "void");
         CHECK(data.lookup(8).lexeme == "void");
         CHECK(data.lookup(8).line == 2);

         CHECK(data.lookup(9).type == "id");
         CHECK(data.lookup(9).lexeme == "my_void");
         CHECK(data.lookup(9).line == 3);

         CHECK(data.lookup(10).type == "public");
         CHECK(data.lookup(10).lexeme == "public");
         CHECK(data.lookup(10).line == 4);

         CHECK(data.lookup(11).type == "private");
         CHECK(data.lookup(11).lexeme == "private");
         CHECK(data.lookup(11).line == 4);
      }
   }
   TEST_CASE("integer test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("test_files/num_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         CHECK(data.lookup(0).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(1).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(2).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(3).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(4).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(5).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(6).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(7).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(8).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(9).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(10).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(11).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(12).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(13).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(14).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(15).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(16).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(17).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(18).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(19).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(20).type == to_string_view(lex::item_type::integer_lit));
         CHECK(data.lookup(21).type == to_string_view(lex::item_type::id));
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
            CHECK(tok.type == to_string_view(lex::item_type::float_lit));
            CHECK(tok.line == index++);
         }
      }
      SUBCASE("weird")
      {
         using namespace lex;

         auto maybe = lex::lex_file("test_files/float_invalid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(data.lookup(0) == item{to_string_view(item_type::integer_lit), "0", 1});
         CHECK(data.lookup(1) == item{to_string_view(item_type::float_lit), "123.134", 1});
         CHECK(data.lookup(2) == item{to_string_view(item_type::integer_lit), "0", 2});
         CHECK(data.lookup(3) == item{to_string_view(item_type::integer_lit), "1", 2});
         CHECK(data.lookup(4) == item{to_string_view(item_type::id), "ab3", 2});
         CHECK(data.lookup(5) == item{to_string_view(item_type::period), ".", 2});
         CHECK(data.lookup(6) == item{to_string_view(item_type::integer_lit), "134", 2});
         CHECK(data.lookup(7) == item{to_string_view(item_type::float_lit), "0.123", 3});
         CHECK(data.lookup(8) == item{to_string_view(item_type::integer_lit), "0", 3});
         CHECK(data.lookup(9) == item{to_string_view(item_type::float_lit), "123.0123", 4});
         CHECK(data.lookup(10) == item{to_string_view(item_type::id), "ab", 4});
         CHECK(data.lookup(11) == item{to_string_view(item_type::float_lit), "123.0123", 5});
         CHECK(data.lookup(12) == item{to_string_view(item_type::id), "ab_", 5});
         CHECK(data.lookup(13) == item{to_string_view(item_type::float_lit), "0.0", 6});
         CHECK(data.lookup(14) == item{to_string_view(item_type::integer_lit), "0", 6});
         CHECK(data.lookup(15) == item{to_string_view(item_type::integer_lit), "0", 6});
         CHECK(data.lookup(16) == item{to_string_view(item_type::integer_lit), "0", 6});
         CHECK(data.lookup(17) == item{to_string_view(item_type::integer_lit), "0", 6});
         CHECK(data.lookup(18) == item{to_string_view(item_type::integer_lit), "0", 7});
         CHECK(data.lookup(19) == item{to_string_view(item_type::float_lit), "1.23", 7});
         CHECK(data.lookup(20) == item{to_string_view(item_type::integer_lit), "0", 8});
         CHECK(data.lookup(21) == item{to_string_view(item_type::float_lit), "12.34", 8});
         CHECK(data.lookup(22) == item{to_string_view(item_type::float_lit), "12.34", 9});
         CHECK(data.lookup(23) == item{to_string_view(item_type::integer_lit), "0", 9});
         CHECK(data.lookup(24) == item{to_string_view(item_type::integer_lit), "0", 10});
         CHECK(data.lookup(25) == item{to_string_view(item_type::float_lit), "12.34", 10});
         CHECK(data.lookup(26) == item{to_string_view(item_type::integer_lit), "0", 10});
         CHECK(data.lookup(27) == item{to_string_view(item_type::integer_lit), "0", 11});
         CHECK(data.lookup(28) == item{to_string_view(item_type::float_lit), "12.34e10", 11});
         CHECK(data.lookup(29) == item{to_string_view(item_type::float_lit), "12.34e0", 12});
         CHECK(data.lookup(30) == item{to_string_view(item_type::integer_lit), "10", 12});
         CHECK(data.lookup(31) == item{to_string_view(item_type::invalid_num), "12.", 13});
         CHECK(data.lookup(32) == item{to_string_view(item_type::id), "ab", 13});
         CHECK(data.lookup(33) == item{to_string_view(item_type::invalid_num), "1.2e-", 14});
         CHECK(data.lookup(34) == item{to_string_view(item_type::id), "a123", 14});
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
            CHECK(tok.type == to_string_view(lex::item_type::str_lit));
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
            CHECK(tok.type == to_string_view(lex::item_type::invalid_str));
         }
      }
   }
   TEST_CASE("punctuation test")
   {
      auto maybe = lex::lex_file("test_files/punctuation_valid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            lex::item{.type = to_string_view(lex::item_type::period), .lexeme = ".", .line = 1});
      CHECK(
         *(std::begin(data) + 1) ==
         lex::item{.type = to_string_view(lex::item_type::semi_colon), .lexeme = ";", .line = 1});
      CHECK(*(std::begin(data) + 2) ==
            lex::item{.type = to_string_view(lex::item_type::comma), .lexeme = ",", .line = 1});
      CHECK(*(std::begin(data) + 3) ==
            lex::item{
               .type = to_string_view(lex::item_type::double_colon), .lexeme = "::", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            lex::item{.type = to_string_view(lex::item_type::colon), .lexeme = ":", .line = 1});
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
               item{.type = to_string_view(item_type::double_equal), .lexeme = "==", .line = 1});
         CHECK(data.lookup(1) ==
               item{.type = to_string_view(item_type::less_equal_than), .lexeme = "<=", .line = 1});
         CHECK(
            data.lookup(2) ==
            item{.type = to_string_view(item_type::greater_equal_than), .lexeme = ">=", .line = 1});
         CHECK(data.lookup(3) ==
               item{.type = to_string_view(item_type::not_equal), .lexeme = "<>", .line = 1});
         CHECK(data.lookup(4) ==
               item{.type = to_string_view(item_type::greater_than), .lexeme = ">", .line = 1});
         CHECK(data.lookup(5) ==
               item{.type = to_string_view(item_type::less_than), .lexeme = "<", .line = 1});
         CHECK(data.lookup(6) ==
               item{.type = to_string_view(item_type::add_op), .lexeme = "+", .line = 3});
         CHECK(data.lookup(7) ==
               item{.type = to_string_view(item_type::sub_op), .lexeme = "-", .line = 3});
         CHECK(data.lookup(8) ==
               item{.type = to_string_view(item_type::mult_op), .lexeme = "*", .line = 4});
         CHECK(data.lookup(9) ==
               item{.type = to_string_view(item_type::div_op), .lexeme = "/", .line = 4});
         CHECK(data.lookup(10) ==
               item{.type = to_string_view(item_type::equal_op), .lexeme = "=", .line = 6});
         CHECK(data.lookup(11) ==
               item{.type = to_string_view(item_type::or_op), .lexeme = "|", .line = 8});
         CHECK(data.lookup(12) ==
               item{.type = to_string_view(item_type::and_op), .lexeme = "&", .line = 8});
         CHECK(data.lookup(13) ==
               item{.type = to_string_view(item_type::not_op), .lexeme = "!", .line = 8});
         CHECK(data.lookup(14) ==
               item{.type = to_string_view(item_type::qmark), .lexeme = "?", .line = 8});
      }
   }
   TEST_CASE("braces test")
   {
      auto maybe = lex::lex_file("test_files/braces_valid.txt");
      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(
         *(std::begin(data) + 0) ==
         lex::item{.type = to_string_view(lex::item_type::open_parenth), .lexeme = "(", .line = 1});
      CHECK(*(std::begin(data) + 1) ==
            lex::item{
               .type = to_string_view(lex::item_type::close_parenth), .lexeme = ")", .line = 1});
      CHECK(
         *(std::begin(data) + 2) ==
         lex::item{.type = to_string_view(lex::item_type::open_curly), .lexeme = "{", .line = 1});
      CHECK(
         *(std::begin(data) + 3) ==
         lex::item{.type = to_string_view(lex::item_type::close_curly), .lexeme = "}", .line = 1});
      CHECK(
         *(std::begin(data) + 4) ==
         lex::item{.type = to_string_view(lex::item_type::open_square), .lexeme = "[", .line = 1});
      CHECK(
         *(std::begin(data) + 5) ==
         lex::item{.type = to_string_view(lex::item_type::close_square), .lexeme = "]", .line = 1});
   }
   TEST_CASE("comment test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::lex_file("test_files/comment_valid.txt");
         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK((std::begin(data) + 0)->type == lex::to_string_view(lex::item_type::line_cmt));
         CHECK((std::begin(data) + 1)->type == lex::to_string_view(lex::item_type::block_cmt));
         CHECK((std::begin(data) + 2)->type == lex::to_string_view(lex::item_type::block_cmt));
         CHECK((std::begin(data) + 3)->type == lex::to_string_view(lex::item_type::block_cmt));
         CHECK((std::begin(data) + 3)->line == 7);
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::lex_file("test_files/comment_invalid.txt");
         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(std::begin(data)->type == lex::to_string_view(lex::item_type::invalid_cmt));
      }
   }
   TEST_CASE("invalid characters")
   {
      auto maybe = lex::lex_file("test_files/chars_invalid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(
         *(std::begin(data) + 0) ==
         lex::item{.type = to_string_view(lex::item_type::invalid_char), .lexeme = "@", .line = 1});
      CHECK(
         *(std::begin(data) + 1) ==
         lex::item{.type = to_string_view(lex::item_type::invalid_char), .lexeme = "#", .line = 1});
      CHECK(
         *(std::begin(data) + 2) ==
         lex::item{.type = to_string_view(lex::item_type::invalid_char), .lexeme = "$", .line = 1});
      CHECK(
         *(std::begin(data) + 3) ==
         lex::item{.type = to_string_view(lex::item_type::invalid_char), .lexeme = "'", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            lex::item{
               .type = to_string_view(lex::item_type::invalid_char), .lexeme = "\\", .line = 1});
      CHECK(
         *(std::begin(data) + 5) ==
         lex::item{.type = to_string_view(lex::item_type::invalid_char), .lexeme = "~", .line = 1});
   }
}
