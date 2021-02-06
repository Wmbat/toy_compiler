#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <toy_compiler/lex/lexer.hpp>

#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/iota.hpp>

namespace vi = ranges::views;

TEST_SUITE("Lexer test suite")
{
   TEST_CASE("non-existant file")
   {
      lexer lex;

      auto maybe = lex.tokenize_file("tests/file_that_doesn't_exist.txt");

      REQUIRE(!maybe);
   }
   TEST_CASE("id test")
   {
      SUBCASE("standard")
      {
         lexer lex;

         auto maybe = lex.tokenize_file("tests/id.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 11);

         for (auto& token : data)
         {
            CHECK(token.tok == to_string(token_type::id));
         }
      }

      SUBCASE("standard with tabs")
      {
         lexer lex;

         auto maybe = lex.tokenize_file("tests/id_tabs.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 11);

         for (auto& token : data)
         {
            CHECK(token.tok == to_string(token_type::id));
         }
      }

      SUBCASE("invalid")
      {
         lexer lex;

         auto maybe = lex.tokenize_file("tests/id_invalid.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 9);
         CHECK(data.lookup(0).tok == to_string(token_type::invalid_id));
         CHECK(data.lookup(0).line == 1);
         CHECK(data.lookup(1).tok == to_string(token_type::id));
         CHECK(data.lookup(1).line == 1);
         CHECK(data.lookup(2).tok == to_string(token_type::id));
         CHECK(data.lookup(2).line == 1);
         CHECK(data.lookup(3).tok == to_string(token_type::id));
         CHECK(data.lookup(3).line == 1);
         CHECK(data.lookup(4).tok == to_string(token_type::id));
         CHECK(data.lookup(4).line == 1);
         CHECK(data.lookup(5).tok == to_string(token_type::id));
         CHECK(data.lookup(6).line == 1);
         CHECK(data.lookup(6).tok == to_string(token_type::invalid_id));
         CHECK(data.lookup(6).line == 1);
         CHECK(data.lookup(7).tok == to_string(token_type::invalid_char));
         CHECK(data.lookup(7).line == 1);
         CHECK(data.lookup(8).tok == to_string(token_type::invalid_id));
         CHECK(data.lookup(8).line == 2);
      }
   }
   TEST_CASE("keyword test")
   {
      SUBCASE("positive keywords")
      {
         lexer lex;

         auto maybe = lex.tokenize_file("tests/keyword_positive.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         for (std::size_t line_counter = 0; auto i : vi::iota(0u, std::size(keywords)))
         {
            if (i % 7 == 0)
            {
               line_counter++;
            }

            CHECK(data.lookup(i).tok == keywords.at(i));
            CHECK(data.lookup(i).lexeme == keywords.at(i));
            CHECK(data.lookup(i).line == line_counter);
         }
      }
      SUBCASE("mixed")
      {
         lexer lex;

         auto maybe = lex.tokenize_file("tests/keyword.txt");

         REQUIRE(maybe.has_value());

         auto data = maybe.value();

         CHECK(std::size(data) == 12);

         CHECK(data.lookup(0).tok == "if");
         CHECK(data.lookup(0).lexeme == "if");
         CHECK(data.lookup(0).line == 1);

         CHECK(data.lookup(1).tok == "then");
         CHECK(data.lookup(1).lexeme == "then");
         CHECK(data.lookup(1).line == 1);

         CHECK(data.lookup(2).tok == "while");
         CHECK(data.lookup(2).lexeme == "while");
         CHECK(data.lookup(2).line == 1);

         CHECK(data.lookup(3).tok == "id");
         CHECK(data.lookup(3).lexeme == "ifthen");
         CHECK(data.lookup(3).line == 1);

         CHECK(data.lookup(4).tok == "id");
         CHECK(data.lookup(4).lexeme == "megatest");
         CHECK(data.lookup(4).line == 1);

         CHECK(data.lookup(5).tok == "integer");
         CHECK(data.lookup(5).lexeme == "integer");
         CHECK(data.lookup(5).line == 1);

         CHECK(data.lookup(6).tok == "float");
         CHECK(data.lookup(6).lexeme == "float");
         CHECK(data.lookup(6).line == 2);

         CHECK(data.lookup(7).tok == "string");
         CHECK(data.lookup(7).lexeme == "string");
         CHECK(data.lookup(7).line == 2);

         CHECK(data.lookup(8).tok == "void");
         CHECK(data.lookup(8).lexeme == "void");
         CHECK(data.lookup(8).line == 2);

         CHECK(data.lookup(9).tok == "id");
         CHECK(data.lookup(9).lexeme == "my_void");
         CHECK(data.lookup(9).line == 3);

         CHECK(data.lookup(10).tok == "public");
         CHECK(data.lookup(10).lexeme == "public");
         CHECK(data.lookup(10).line == 4);

         CHECK(data.lookup(11).tok == "private");
         CHECK(data.lookup(11).lexeme == "private");
         CHECK(data.lookup(11).line == 4);
      }
   }
   TEST_CASE("integer test")
   {
      SUBCASE("valid")
      {
         lexer lex;

         auto maybe = lex.tokenize_file("tests/num.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : data)
         {
            CHECK(tok.tok == to_string(token_type::integer_lit));
            CHECK(tok.line == index++);
         }
      }

      SUBCASE("invalid")
      {
         lexer lex;

         auto maybe = lex.tokenize_file("tests/num_invalid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : data)
         {
            CHECK(tok.tok == to_string(token_type::invalid_num));
            CHECK(tok.line == index++);
         }
      }
   }
   TEST_CASE("float test")
   {
      SUBCASE("valid")
      {
         lexer lex;

         auto maybe = lex.tokenize_file("tests/float_valid.txt");

         REQUIRE(maybe);

         auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : data)
         {
            if (index < 5)
            {
               CHECK(tok.tok == to_string(token_type::float_lit));
               CHECK(tok.line == index++);
            }
         }

         CHECK((std::end(data) - 2)->tok == to_string(token_type::float_lit));
         CHECK((std::end(data) - 2)->line == 5);
         CHECK((std::end(data) - 1)->tok == to_string(token_type::invalid_id));
         CHECK((std::end(data) - 1)->line == 5);
      }
      SUBCASE("invalid")
      {
         lexer lex;

         auto maybe = lex.tokenize_file("tests/float_invalid.txt");

         REQUIRE(maybe);

         const auto data = maybe.value();

         for (std::uint32_t index = 1; auto& tok : data)
         {
            CHECK(tok.tok == to_string(token_type::invalid_num));
            CHECK(tok.line == index++);
         }
      }
   }
   TEST_CASE("punctuation test")
   {
      lexer lex;

      auto maybe = lex.tokenize_file("tests/punctuation_valid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            token{.tok = to_string(token_type::period), .lexeme = ".", .line = 1});
      CHECK(*(std::begin(data) + 1) ==
            token{.tok = to_string(token_type::semi_colon), .lexeme = ";", .line = 1});
      CHECK(*(std::begin(data) + 2) ==
            token{.tok = to_string(token_type::comma), .lexeme = ",", .line = 1});
      CHECK(*(std::begin(data) + 3) ==
            token{.tok = to_string(token_type::double_colon), .lexeme = "::", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            token{.tok = to_string(token_type::colon), .lexeme = ":", .line = 1});
   }
   TEST_CASE("invalid characters")
   {
      lexer lex;

      auto maybe = lex.tokenize_file("tests/chars_invalid.txt");

      REQUIRE(maybe);

      const auto data = maybe.value();

      CHECK(*(std::begin(data) + 0) ==
            token{.tok = to_string(token_type::invalid_char), .lexeme = "@", .line = 1});
      CHECK(*(std::begin(data) + 1) ==
            token{.tok = to_string(token_type::invalid_char), .lexeme = "#", .line = 1});
      CHECK(*(std::begin(data) + 2) ==
            token{.tok = to_string(token_type::invalid_char), .lexeme = "$", .line = 1});
      CHECK(*(std::begin(data) + 3) ==
            token{.tok = to_string(token_type::invalid_char), .lexeme = "'", .line = 1});
      CHECK(*(std::begin(data) + 4) ==
            token{.tok = to_string(token_type::invalid_char), .lexeme = "\\", .line = 1});
      CHECK(*(std::begin(data) + 5) ==
            token{.tok = to_string(token_type::invalid_char), .lexeme = "~", .line = 1});
   }
}
