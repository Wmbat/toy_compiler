/**
 * @file application.hpp
 * @brief Contains the primary entry point for the compiler and all the compiling facilities
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

#include <toy_compiler/munster/lexer.hpp>
#include <toy_compiler/munster/parser.hpp>
#include <toy_compiler/util/logger.hpp>

#include <filesystem>
#include <span>

/**
 * @brief The main entry point for the compiler
 */
class application
{
public:
   application(std::span<const std::string_view> args, util::logger_wrapper log = nullptr);

private:
   void write_lexing_to_file(const std::filesystem::path& path,
                             const std::span<munster::lex_item>& items) const;
   void write_moon_code_to_file(const std::filesystem::path& path, std::string_view code) const;
   void write_derivations_to_file(const std::filesystem::path& path,
                                  const std::string& derivation) const;
   void write_ast_to_file(const std::filesystem::path& path,
                          const munster::ast::node_ptr& root) const;
   void write_symbol_tables_to_file(const std::filesystem::path& path,
                                    const munster::symbol_table* root) const;

   [[nodiscard]] auto fancy_lexical_error_type(munster::grammar::token_type value) const
      -> std::string;

private:
   util::logger_wrapper m_logger;
};
