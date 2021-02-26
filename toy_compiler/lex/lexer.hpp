/**
 * @file lexer.hpp
 * @brief Contains all the lexing functions for the compiler
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

#pragma once

#include <toy_compiler/lex/item.hpp>
#include <toy_compiler/util/logger.hpp>

#include <libcaramel/containers/dynamic_array.hpp>

#include <monads/maybe.hpp>

#include <filesystem>

namespace lex
{
   /**
    * @brief Opens a file and lexes it
    *
    * @param [in] path The path to the file
    * @param [in] log A logger used for status information
    *
    * @return The list of `lex::item` from the file
    */
   auto lex_file(const std::filesystem::path& path, util::logger_wrapper log = nullptr)
      -> monad::maybe<crl::dynamic_array<lex::item>>;
} // namespace lex
