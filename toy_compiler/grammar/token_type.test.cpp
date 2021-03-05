/**
 * @file token_type.test.cpp
 * @brief Runs tests on functions defined in token_type.hpp
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

#include <toy_compiler/grammar/token_type.hpp>

TEST_SUITE("grammar/token_type.hpp test suite")
{
   TEST_CASE("to_string")
   {
      using namespace grammar;

      CHECK(to_string_view(token_type::none) == "none");
      CHECK(to_string_view(token_type::id) == "id");
      CHECK(to_string_view(token_type::integer_lit) == "integer_lit");
      CHECK(to_string_view(token_type::float_lit) == "float_lit");
      CHECK(to_string_view(token_type::str_lit) == "str_lit");
      CHECK(to_string_view(token_type::period) == "period");
      CHECK(to_string_view(token_type::comma) == "comma");
      CHECK(to_string_view(token_type::colon) == "colon");
      CHECK(to_string_view(token_type::double_colon) == "double_colon");
      CHECK(to_string_view(token_type::semi_colon) == "semi_colon");
      CHECK(to_string_view(token_type::add_op) == "add");
      CHECK(to_string_view(token_type::sub_op) == "sub");
      CHECK(to_string_view(token_type::mult_op) == "mult");
      CHECK(to_string_view(token_type::div_op) == "div");
      CHECK(to_string_view(token_type::equal_op) == "equal");
      CHECK(to_string_view(token_type::or_op) == "or");
      CHECK(to_string_view(token_type::and_op) == "and");
      CHECK(to_string_view(token_type::not_op) == "not");
      CHECK(to_string_view(token_type::qmark) == "qmark");
      CHECK(to_string_view(token_type::double_equal) == "double_equal");
      CHECK(to_string_view(token_type::less_than) == "less_than");
      CHECK(to_string_view(token_type::less_equal_than) == "less_equal_than");
      CHECK(to_string_view(token_type::greater_than) == "greater_than");
      CHECK(to_string_view(token_type::greater_equal_than) == "greater_equal_than");
      CHECK(to_string_view(token_type::not_equal) == "not_equal");
      CHECK(to_string_view(token_type::left_brace) == "left_brace");
      CHECK(to_string_view(token_type::right_brace) == "right_brace");
      CHECK(to_string_view(token_type::left_square) == "left_square");
      CHECK(to_string_view(token_type::right_square) == "right_square");
      CHECK(to_string_view(token_type::left_paren) == "left_paren");
      CHECK(to_string_view(token_type::right_paren) == "right_paren");
      CHECK(to_string_view(token_type::line_cmt) == "line_cmt");
      CHECK(to_string_view(token_type::block_cmt) == "block_cmt");
      CHECK(to_string_view(token_type::invalid_char) == "invalid_char");
      CHECK(to_string_view(token_type::invalid_id) == "invalid_id");
      CHECK(to_string_view(token_type::invalid_num) == "invalid_num");
      CHECK(to_string_view(token_type::invalid_str) == "invalid_str");
      CHECK(to_string_view(token_type::invalid_cmt) == "invalid_cmt");
   }
   TEST_CASE("fmt::formatter - token_type")
   {
      using namespace grammar;

      CHECK(fmt::format("{}", token_type::none) == "none");
      CHECK(fmt::format("{}", token_type::id) == "id");
      CHECK(fmt::format("{}", token_type::integer_lit) == "integer_lit");
      CHECK(fmt::format("{}", token_type::float_lit) == "float_lit");
      CHECK(fmt::format("{}", token_type::str_lit) == "str_lit");
      CHECK(fmt::format("{}", token_type::period) == "period");
      CHECK(fmt::format("{}", token_type::comma) == "comma");
      CHECK(fmt::format("{}", token_type::colon) == "colon");
      CHECK(fmt::format("{}", token_type::double_colon) == "double_colon");
      CHECK(fmt::format("{}", token_type::semi_colon) == "semi_colon");
      CHECK(fmt::format("{}", token_type::add_op) == "add");
      CHECK(fmt::format("{}", token_type::sub_op) == "sub");
      CHECK(fmt::format("{}", token_type::mult_op) == "mult");
      CHECK(fmt::format("{}", token_type::div_op) == "div");
      CHECK(fmt::format("{}", token_type::equal_op) == "equal");
      CHECK(fmt::format("{}", token_type::or_op) == "or");
      CHECK(fmt::format("{}", token_type::and_op) == "and");
      CHECK(fmt::format("{}", token_type::not_op) == "not");
      CHECK(fmt::format("{}", token_type::qmark) == "qmark");
      CHECK(fmt::format("{}", token_type::double_equal) == "double_equal");
      CHECK(fmt::format("{}", token_type::less_than) == "less_than");
      CHECK(fmt::format("{}", token_type::less_equal_than) == "less_equal_than");
      CHECK(fmt::format("{}", token_type::greater_than) == "greater_than");
      CHECK(fmt::format("{}", token_type::greater_equal_than) == "greater_equal_than");
      CHECK(fmt::format("{}", token_type::not_equal) == "not_equal");
      CHECK(fmt::format("{}", token_type::left_brace) == "left_brace");
      CHECK(fmt::format("{}", token_type::right_brace) == "right_brace");
      CHECK(fmt::format("{}", token_type::left_square) == "left_square");
      CHECK(fmt::format("{}", token_type::right_square) == "right_square");
      CHECK(fmt::format("{}", token_type::left_paren) == "left_paren");
      CHECK(fmt::format("{}", token_type::right_paren) == "right_paren");
      CHECK(fmt::format("{}", token_type::line_cmt) == "line_cmt");
      CHECK(fmt::format("{}", token_type::block_cmt) == "block_cmt");
      CHECK(fmt::format("{}", token_type::invalid_char) == "invalid_char");
      CHECK(fmt::format("{}", token_type::invalid_id) == "invalid_id");
      CHECK(fmt::format("{}", token_type::invalid_num) == "invalid_num");
      CHECK(fmt::format("{}", token_type::invalid_str) == "invalid_str");
      CHECK(fmt::format("{}", token_type::invalid_cmt) == "invalid_cmt");
   }
}
