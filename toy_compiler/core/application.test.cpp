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

TEST_SUITE("Application test suite")
{
   TEST_CASE("valid")
   {
      SUBCASE("polynomial")
      {
         std::string filepath = "test_files/polynomial.src";
         std::string output_symbol_table = "test_files/polynomial.outsymboltables";

         std::vector<std::string_view> filepaths{filepath};
         application app{filepaths};

         std::filesystem::path file_test{filepath};
         std::filesystem::path output_test{output_symbol_table};

         CHECK(std::filesystem::exists(file_test));
         CHECK(std::filesystem::exists(output_test));
      }
   }
   TEST_CASE("error")
   {
      std::string filepath = "test_files/polynomialsemanticerrors.src";
      std::string output_symbol_table = "test_files/polynomialsemanticerrors.outsymboltables";

      std::vector<std::string_view> filepaths{filepath};
      application app{filepaths};

      std::filesystem::path file_test{filepath};
      std::filesystem::path output_test{output_symbol_table};

      CHECK(std::filesystem::exists(file_test));
      CHECK(std::filesystem::exists(output_test));
   }
}
