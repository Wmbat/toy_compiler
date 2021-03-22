/**
 * @file toy_compiler.cpp
 * @brief Entry point for the toy_compiler project.
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
 *
 * @mainpage
 *
 * Compiler build for my **COMP 442: Compiler Design** class in University.
 *
 * @page grammar
 *
 * The language this project compiles is using the following LL(1) grammar
 */

#include <toy_compiler/core/application.hpp>
#include <toy_compiler/util/logger.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/tail.hpp>

#include <easy/profiler.h>

namespace views = ranges::views;

auto main(int argc, char** argv) -> int
{
   using args_array = std::vector<std::string_view>;

   // clang-format off
   args_array arguments = std::span<char*>{argv, static_cast<std::size_t>(argc)} 
      | views::tail // first argument is program name.
      | ranges::to<args_array>;
   // clang-format on 

   std::string logger_name = "toy_compiler";
   util::logger logger{logger_name};

   application app{arguments, &logger};

   return 0;
}
