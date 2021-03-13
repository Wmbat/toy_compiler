/**
 * @file application.cpp
 * @brief Implement the functions defined at application.hpp
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

#include <toy_compiler/core/application.hpp>

#include <toy_compiler/front_end/parser.hpp>

#include <fmt/color.h>
#include <fmt/ranges.h>

#include <range/v3/algorithm/count_if.hpp>

#include <cassert>
#include <fstream>

namespace fs = std::filesystem;

application::application(std::span<const std::string_view> args, util::logger_wrapper log) :
   m_logger{log}
{
   for (const auto filename : args)
   {
      const auto filepath = fs::path{filename};
      if (filepath.extension() == ".src")
      {
         if (auto maybe = fr::lex_file(filepath, m_logger))
         {
            const auto result = fr::parse_items(maybe.value(), m_logger);
            if (result.value == fr::parse_status::error)
            {
               for (auto& err : result.errors.value())
               {
                  fmt::print(fmt::emphasis::bold, "{}:{}.{} - ", filepath.c_str(), err.pos.line,
                             err.pos.column);
                  fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "[error]");

                  if (err.type == fr::parse_error_type::missing_terminal)
                  {
                     fmt::print(" Missing {}\n", err.token);
                  }

                  fmt::print("\n");
               }
            }
         }
         else
         {
            fmt::print("failed to open file \"{}\". Doublecheck filepath\n", filepath.c_str());

            m_logger.warning(
               "Failed to open file located at \"{}\". Make sure the filepath is valid",
               filepath.c_str());
         }
      }
      else
      {
         fmt::print("file \"{}\" does not have the correct file extension (.src)\n",
                    filepath.c_str());

         m_logger.warning("File \"{}\" does not have the correct file extension (.src)",
                          filepath.c_str());
      }
   }
}

void application::write_tokens_to_file(const std::filesystem::path& path,
                                       std::span<const fr::lex_item> tokens,
                                       util::logger_wrapper log) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outlextokens";

   std::string outlextoken;
   for (std::uint32_t line_counter = 1; const auto& tok : tokens)
   {
      for (std::uint32_t i = line_counter; i < tok.pos.line; ++i)
      {
         outlextoken.append("\n");
      }

      outlextoken.append(fmt::format("{} ", tok));

      line_counter = tok.pos.line;
   }

   std::ofstream output_file{output_path};
   output_file << outlextoken;

   log.info("writing tokens to \"{}\"", output_path.c_str());
}

void application::write_errors_to_file(const std::filesystem::path& path,
                                       [[maybe_unused]] std::span<const fr::lex_item> tokens,
                                       [[maybe_unused]] util::logger_wrapper log) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outlexerrors";

   std::string outlexerrors;
   for (const auto& tok : tokens)
   {
      if (fr::grammar::is_token_invalid(tok.type))
      {
         outlexerrors.append(fmt::format("Lexical error: {}: \"{}\": line {}.\n",
                                         fancy_lexical_error_type(tok.type), tok.lexeme,
                                         tok.pos.line));
      }
   }

   if (!outlexerrors.empty())
   {
      std::ofstream output_file{output_path};
      output_file << outlexerrors;

      log.warning("writing error messages to \"{}\"", output_path.c_str());
   }
   else
   {
      log.info("no errors found");
   }
}

auto application::fancy_lexical_error_type(fr::grammar::token_type value) const -> std::string
{
   // NOLINTNEXTLINE
   assert(fr::grammar::is_token_invalid(value) && "Only invalid types should be passed");

   using namespace fr::grammar;

   if (value == token_type::invalid_char)
   {
      return "invalid character";
   }

   if (value == token_type::invalid_id)
   {
      return "invalid identifier";
   }

   if (value == token_type::invalid_num)
   {
      return "invalid number literal";
   }

   if (value == token_type::invalid_str)
   {
      return "invalid string literal";
   }

   return "invalid comment";
}
