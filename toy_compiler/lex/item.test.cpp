#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>

#include <toy_compiler/lex/item.hpp>

#include <fmt/format.h>

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

TEST_SUITE("Token test suite")
{
   TEST_CASE("to_string")
   {
      using namespace lex;

      CHECK(to_string_view(item_type::none) == "none");
      CHECK(to_string_view(item_type::id) == "id");
      CHECK(to_string_view(item_type::integer_lit) == "integer_lit");
      CHECK(to_string_view(item_type::float_lit) == "float_lit");
      CHECK(to_string_view(item_type::str_lit) == "str_lit");
      CHECK(to_string_view(item_type::period) == "period");
      CHECK(to_string_view(item_type::comma) == "comma");
      CHECK(to_string_view(item_type::colon) == "colon");
      CHECK(to_string_view(item_type::double_colon) == "double_colon");
      CHECK(to_string_view(item_type::semi_colon) == "semi_colon");
      CHECK(to_string_view(item_type::add_op) == "add");
      CHECK(to_string_view(item_type::sub_op) == "sub");
      CHECK(to_string_view(item_type::mult_op) == "mult");
      CHECK(to_string_view(item_type::div_op) == "div");
      CHECK(to_string_view(item_type::equal_op) == "equal");
      CHECK(to_string_view(item_type::or_op) == "or");
      CHECK(to_string_view(item_type::and_op) == "and");
      CHECK(to_string_view(item_type::not_op) == "not");
      CHECK(to_string_view(item_type::qmark) == "qmark");
      CHECK(to_string_view(item_type::double_equal) == "double_equal");
      CHECK(to_string_view(item_type::less_than) == "less_than");
      CHECK(to_string_view(item_type::less_equal_than) == "less_equal_than");
      CHECK(to_string_view(item_type::greater_than) == "greater_than");
      CHECK(to_string_view(item_type::greater_equal_than) == "greater_equal_than");
      CHECK(to_string_view(item_type::not_equal) == "not_equal");
      CHECK(to_string_view(item_type::open_curly) == "open_curly");
      CHECK(to_string_view(item_type::close_curly) == "close_curly");
      CHECK(to_string_view(item_type::open_square) == "open_square");
      CHECK(to_string_view(item_type::close_square) == "close_square");
      CHECK(to_string_view(item_type::open_parenth) == "open_parenth");
      CHECK(to_string_view(item_type::close_parenth) == "close_parenth");
      CHECK(to_string_view(item_type::line_cmt) == "line_cmt");
      CHECK(to_string_view(item_type::block_cmt) == "block_cmt");
      CHECK(to_string_view(item_type::invalid_char) == "invalid_char");
      CHECK(to_string_view(item_type::invalid_id) == "invalid_id");
      CHECK(to_string_view(item_type::invalid_num) == "invalid_num");
      CHECK(to_string_view(item_type::invalid_str) == "invalid_str");
      CHECK(to_string_view(item_type::invalid_cmt) == "invalid_cmt");
   }
   TEST_CASE("fmt::formatter - token_type")
   {
      using namespace lex;

      CHECK(fmt::format("{}", item_type::none) == "none");
      CHECK(fmt::format("{}", item_type::id) == "id");
      CHECK(fmt::format("{}", item_type::integer_lit) == "integer_lit");
      CHECK(fmt::format("{}", item_type::float_lit) == "float_lit");
      CHECK(fmt::format("{}", item_type::str_lit) == "str_lit");
      CHECK(fmt::format("{}", item_type::period) == "period");
      CHECK(fmt::format("{}", item_type::comma) == "comma");
      CHECK(fmt::format("{}", item_type::colon) == "colon");
      CHECK(fmt::format("{}", item_type::double_colon) == "double_colon");
      CHECK(fmt::format("{}", item_type::semi_colon) == "semi_colon");
      CHECK(fmt::format("{}", item_type::add_op) == "add");
      CHECK(fmt::format("{}", item_type::sub_op) == "sub");
      CHECK(fmt::format("{}", item_type::mult_op) == "mult");
      CHECK(fmt::format("{}", item_type::div_op) == "div");
      CHECK(fmt::format("{}", item_type::equal_op) == "equal");
      CHECK(fmt::format("{}", item_type::or_op) == "or");
      CHECK(fmt::format("{}", item_type::and_op) == "and");
      CHECK(fmt::format("{}", item_type::not_op) == "not");
      CHECK(fmt::format("{}", item_type::qmark) == "qmark");
      CHECK(fmt::format("{}", item_type::double_equal) == "double_equal");
      CHECK(fmt::format("{}", item_type::less_than) == "less_than");
      CHECK(fmt::format("{}", item_type::less_equal_than) == "less_equal_than");
      CHECK(fmt::format("{}", item_type::greater_than) == "greater_than");
      CHECK(fmt::format("{}", item_type::greater_equal_than) == "greater_equal_than");
      CHECK(fmt::format("{}", item_type::not_equal) == "not_equal");
      CHECK(fmt::format("{}", item_type::open_curly) == "open_curly");
      CHECK(fmt::format("{}", item_type::close_curly) == "close_curly");
      CHECK(fmt::format("{}", item_type::open_square) == "open_square");
      CHECK(fmt::format("{}", item_type::close_square) == "close_square");
      CHECK(fmt::format("{}", item_type::open_parenth) == "open_parenth");
      CHECK(fmt::format("{}", item_type::close_parenth) == "close_parenth");
      CHECK(fmt::format("{}", item_type::line_cmt) == "line_cmt");
      CHECK(fmt::format("{}", item_type::block_cmt) == "block_cmt");
      CHECK(fmt::format("{}", item_type::invalid_char) == "invalid_char");
      CHECK(fmt::format("{}", item_type::invalid_id) == "invalid_id");
      CHECK(fmt::format("{}", item_type::invalid_num) == "invalid_num");
      CHECK(fmt::format("{}", item_type::invalid_str) == "invalid_str");
      CHECK(fmt::format("{}", item_type::invalid_cmt) == "invalid_cmt");
   }
   TEST_CASE("fmt::formatter - token")
   {
      using namespace lex;

      item tok{.type = to_string_view(item_type::integer_lit), .lexeme = "hello", .line = 10};

      CHECK(fmt::format("{}", tok) == fmt::format("[{}, {}, {}]", tok.type, tok.lexeme, tok.line));
   }
}
