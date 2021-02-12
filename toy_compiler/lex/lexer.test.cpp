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
      auto maybe = lex::tokenize_file("test_files/file_that_doesn't_exist.txt");

      REQUIRE(!maybe);
   }
   TEST_CASE("id test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::tokenize_file("test_files/id_valid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 22);

         for (auto& token : data)
         {
            CHECK(token.type == to_string_view(lex::token_type::id));
         }
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::tokenize_file("test_files/id_invalid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 9);
         CHECK(data.lookup(0).type == to_string_view(lex::token_type::invalid_id));
         CHECK(data.lookup(0).line == 1);
         CHECK(data.lookup(1).type == to_string_view(lex::token_type::id));
         CHECK(data.lookup(1).line == 1);
         CHECK(data.lookup(2).type == to_string_view(lex::token_type::id));
         CHECK(data.lookup(2).line == 1);
         CHECK(data.lookup(3).type == to_string_view(lex::token_type::id));
         CHECK(data.lookup(3).line == 1);
         CHECK(data.lookup(4).type == to_string_view(lex::token_type::id));
         CHECK(data.lookup(4).line == 1);
         CHECK(data.lookup(5).type == to_string_view(lex::token_type::id));
         CHECK(data.lookup(6).line == 1);
         CHECK(data.lookup(6).type == to_string_view(lex::token_type::invalid_id));
         CHECK(data.lookup(6).line == 1);
         CHECK(data.lookup(7).type == to_string_view(lex::token_type::invalid_char));
         CHECK(data.lookup(7).line == 1);
         CHECK(data.lookup(8).type == to_string_view(lex::token_type::invalid_id));
         CHECK(data.lookup(8).line == 2);
      }
   }
   TEST_CASE("keyword test")
   {
      SUBCASE("positive lex::keywords")
      {
         auto maybe = lex::tokenize_file("test_files/keyword_valid.txt");

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
         auto maybe = lex::tokenize_file("test_files/keyword_invalid.txt");

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
         auto maybe = lex::tokenize_file("test_files/num_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : data)
         {
            CHECK(tok.type == to_string_view(lex::token_type::integer_lit));
            CHECK(tok.line == index++);
         }
      }

      SUBCASE("invalid")
      {
         auto maybe = lex::tokenize_file("test_files/num_invalid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : data)
         {
            CHECK(tok.type == to_string_view(lex::token_type::invalid_num));
            CHECK(tok.line == index++);
         }
      }
   }
   TEST_CASE("float test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::tokenize_file("test_files/float_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : data)
         {
            CHECK(tok.type == to_string_view(lex::token_type::float_lit));
            CHECK(tok.line == index++);
         }
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::tokenize_file("test_files/float_invalid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : data)
         {
            CHECK(tok.type == to_string_view(lex::token_type::invalid_num));
            CHECK(tok.line == index++);
         }
      }
   }
   TEST_CASE("string test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::tokenize_file("test_files/string_valid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         for (auto& tok : data)
         {
            CHECK(tok.type == to_string_view(lex::token_type::str_lit));
         }
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::tokenize_file("test_files/string_invalid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(std::size(data) == 3);

         for (auto& tok : data)
         {
            CHECK(tok.type == to_string_view(lex::token_type::invalid_str));
         }
      }
   }
   TEST_CASE("punctuation test")
   {
      auto maybe = lex::tokenize_file("test_files/punctuation_valid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            lex::token{.type = to_string_view(lex::token_type::period), .lexeme = ".", .line = 1});
      CHECK(
         *(std::begin(data) + 1) ==
         lex::token{.type = to_string_view(lex::token_type::semi_colon), .lexeme = ";", .line = 1});
      CHECK(*(std::begin(data) + 2) ==
            lex::token{.type = to_string_view(lex::token_type::comma), .lexeme = ",", .line = 1});
      CHECK(*(std::begin(data) + 3) ==
            lex::token{
               .type = to_string_view(lex::token_type::double_colon), .lexeme = "::", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            lex::token{.type = to_string_view(lex::token_type::colon), .lexeme = ":", .line = 1});
   }
   TEST_CASE("operator test")
   {
      SUBCASE("valid")
      {
         using namespace lex;

         auto maybe = tokenize_file("test_files/operators_valid.txt");
         REQUIRE(maybe);
         const auto data = maybe.value();

         CHECK(data.lookup(0) ==
               token{.type = to_string_view(token_type::double_equal), .lexeme = "==", .line = 1});
         CHECK(
            data.lookup(1) ==
            token{.type = to_string_view(token_type::less_equal_than), .lexeme = "<=", .line = 1});
         CHECK(
            data.lookup(2) ==
            token{
               .type = to_string_view(token_type::greater_equal_than), .lexeme = ">=", .line = 1});
         CHECK(data.lookup(3) ==
               token{.type = to_string_view(token_type::not_equal), .lexeme = "<>", .line = 1});
         CHECK(data.lookup(4) ==
               token{.type = to_string_view(token_type::greater_than), .lexeme = ">", .line = 1});
         CHECK(data.lookup(5) ==
               token{.type = to_string_view(token_type::less_than), .lexeme = "<", .line = 1});
         CHECK(data.lookup(6) ==
               token{.type = to_string_view(token_type::add), .lexeme = "+", .line = 3});
         CHECK(data.lookup(7) ==
               token{.type = to_string_view(token_type::sub), .lexeme = "-", .line = 3});
         CHECK(data.lookup(8) ==
               token{.type = to_string_view(token_type::mult), .lexeme = "*", .line = 4});
         CHECK(data.lookup(9) ==
               token{.type = to_string_view(token_type::div), .lexeme = "/", .line = 4});
         CHECK(data.lookup(10) ==
               token{.type = to_string_view(token_type::equal), .lexeme = "=", .line = 6});
         CHECK(data.lookup(11) ==
               token{.type = to_string_view(token_type::or_op), .lexeme = "|", .line = 8});
         CHECK(data.lookup(12) ==
               token{.type = to_string_view(token_type::and_op), .lexeme = "&", .line = 8});
         CHECK(data.lookup(13) ==
               token{.type = to_string_view(token_type::not_op), .lexeme = "!", .line = 8});
         CHECK(data.lookup(14) ==
               token{.type = to_string_view(token_type::qmark), .lexeme = "?", .line = 8});
      }
   }
   TEST_CASE("braces test")
   {
      auto maybe = lex::tokenize_file("test_files/braces_valid.txt");
      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            lex::token{
               .type = to_string_view(lex::token_type::open_parenth), .lexeme = "(", .line = 1});
      CHECK(*(std::begin(data) + 1) ==
            lex::token{
               .type = to_string_view(lex::token_type::close_parenth), .lexeme = ")", .line = 1});
      CHECK(
         *(std::begin(data) + 2) ==
         lex::token{.type = to_string_view(lex::token_type::open_curly), .lexeme = "{", .line = 1});
      CHECK(*(std::begin(data) + 3) ==
            lex::token{
               .type = to_string_view(lex::token_type::close_curly), .lexeme = "}", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            lex::token{
               .type = to_string_view(lex::token_type::open_square), .lexeme = "[", .line = 1});
      CHECK(*(std::begin(data) + 5) ==
            lex::token{
               .type = to_string_view(lex::token_type::close_square), .lexeme = "]", .line = 1});
   }
   TEST_CASE("comment test")
   {
      SUBCASE("valid")
      {
         auto maybe = lex::tokenize_file("test_files/comment_valid.txt");
         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK((std::begin(data) + 0)->type == lex::to_string_view(lex::token_type::line_cmt));
         CHECK((std::begin(data) + 1)->type == lex::to_string_view(lex::token_type::block_cmt));
         CHECK((std::begin(data) + 2)->type == lex::to_string_view(lex::token_type::block_cmt));
         CHECK((std::begin(data) + 3)->type == lex::to_string_view(lex::token_type::block_cmt));
         CHECK((std::begin(data) + 3)->line == 7);
      }
      SUBCASE("invalid")
      {
         auto maybe = lex::tokenize_file("test_files/comment_invalid.txt");
         REQUIRE(maybe);

         const auto data = maybe.value();

         CHECK(std::begin(data)->type == lex::to_string_view(lex::token_type::invalid_cmt));
      }
   }
   TEST_CASE("invalid characters")
   {
      auto maybe = lex::tokenize_file("test_files/chars_invalid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            lex::token{
               .type = to_string_view(lex::token_type::invalid_char), .lexeme = "@", .line = 1});
      CHECK(*(std::begin(data) + 1) ==
            lex::token{
               .type = to_string_view(lex::token_type::invalid_char), .lexeme = "#", .line = 1});
      CHECK(*(std::begin(data) + 2) ==
            lex::token{
               .type = to_string_view(lex::token_type::invalid_char), .lexeme = "$", .line = 1});
      CHECK(*(std::begin(data) + 3) ==
            lex::token{
               .type = to_string_view(lex::token_type::invalid_char), .lexeme = "'", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            lex::token{
               .type = to_string_view(lex::token_type::invalid_char), .lexeme = "\\", .line = 1});
      CHECK(*(std::begin(data) + 5) ==
            lex::token{
               .type = to_string_view(lex::token_type::invalid_char), .lexeme = "~", .line = 1});
   }
}
