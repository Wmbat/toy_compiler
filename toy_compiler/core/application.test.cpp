/**
 * @brief Runs tests on functions defined in application.hpp
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

#include <toy_compiler/core/application.hpp>

#include <filesystem>

/*
TEST_SUITE("Application test suite")
{
   TEST_CASE("valid")
   {
   std::string filepath = "test_files/lexpositive.src";
   std::string output_success = "test_files/lexpositive.outlextokens";

   crl::dynamic_array<std::string_view> filepaths{filepath};
   application app{filepaths};

   std::filesystem::path file_test{filepath};
   std::filesystem::path output_test{output_success};

   CHECK(std::filesystem::exists(file_test));
   CHECK(std::filesystem::exists(output_test));
   }
   TEST_CASE("invalid")
   {
   std::string filepath = "test_files/lexnegative.src";
   std::string output_success = "test_files/lexpositive.outlextokens";
   std::string output_error = "test_files/lexpositive.outlexerrors";

   crl::dynamic_array<std::string_view> filepaths{filepath};
   application app{filepaths};

   std::filesystem::path file_test{filepath};
   std::filesystem::path output_success_test{output_success};
   std::filesystem::path output_error_test{output_success};

   CHECK(std::filesystem::exists(file_test));
   CHECK(std::filesystem::exists(output_success_test));
   CHECK(std::filesystem::exists(output_error_test));
   }
   TEST_CASE("Multiple files")
   {
   std::string filepath_pos = "test_files/lexpositive.src";
   std::string filepath_neg = "test_files/lexnegative.src";

   crl::dynamic_array<std::string_view> filepaths{filepath_pos, filepath_neg};
   application app{filepaths};

   std::filesystem::path file_pos_test{filepath_pos};
   std::filesystem::path file_tokens_pos{"test_files/lexpositive.outlextokens"};

   std::filesystem::path file_neg_test{filepath_pos};
   std::filesystem::path file_tokens_neg{"test_files/lexnegative.outlextokens"};
   std::filesystem::path file_errors_neg{"test_files/lexnegative.outlexerrors"};

   CHECK(std::filesystem::exists(file_pos_test));
   CHECK(std::filesystem::exists(file_tokens_pos));
   CHECK(std::filesystem::exists(file_neg_test));
   CHECK(std::filesystem::exists(file_tokens_neg));
   CHECK(std::filesystem::exists(file_errors_neg));
   }
   }
   */
