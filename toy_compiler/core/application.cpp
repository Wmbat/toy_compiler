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
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <range/v3/algorithm/count_if.hpp>

#include <cassert>
#include <fstream>

namespace fs = std::filesystem;

auto pop(std::vector<front::ast_bis::node*>& stack) -> front::ast_bis::node*
{
   auto temp = *(std::end(stack) - 1);
   stack.pop_back();
   return temp;
}

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

               write_errors_to_file(filepath, result.errors.value());
            }

            write_ast_to_file(filepath, result.ast);
            write_derivations_to_file(filepath, result.derivation);
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

void application::write_derivations_to_file(const std::filesystem::path& path,
                                            const std::string& derivation) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outderivation";

   std::ofstream output_file{output_path};
   fmt::print(output_file, "{}", derivation);
}
void application::write_ast_to_file(const std::filesystem::path& path,
                                    const front::ast_bis::node_ptr& root) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outast";

   std::ofstream output_file{output_path};

   std::vector<front::ast_bis::node*> stack;

   front::ast_bis::node* curr = root.get();
   while (!std::empty(stack) || curr)
   {
      if (curr)
      {
         const auto name = fmt::format("- {}\n", *curr);
         fmt::print("{:>{}}", name, std::size(name) + std::size(stack) * 2);

         stack.push_back(curr);
         curr = curr->child().get();
      }
      else
      {
         curr = pop(stack);
         curr = curr->sibling().get();
      };
   }
}
void application::write_errors_to_file(const std::filesystem::path& path,
                                       std::span<const fr::parse_error> errors) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outsyntaxerrors";

   std::ofstream output_file{output_path};
   for (auto& err : errors)
   {
      fmt::print(output_file, "{}:{}.{} - ", path.c_str(), err.pos.line, err.pos.column);
      fmt::print(output_file, "[error]");

      if (err.type == fr::parse_error_type::missing_terminal)
      {
         fmt::print(output_file, " Missing {}\n", err.token);
      }

      fmt::print("\n");
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
