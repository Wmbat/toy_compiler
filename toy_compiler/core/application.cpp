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

#include <toy_compiler/front_end/ast/visitor/symbol_table_visitor.hpp>
#include <toy_compiler/front_end/ast/visitor/visitor.hpp>
#include <toy_compiler/front_end/lexer.hpp>
#include <toy_compiler/front_end/parser.hpp>

#include <fmt/color.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/view/map.hpp>

#include <cassert>
#include <fstream>

namespace fs = std::filesystem;

auto pop(std::vector<front::ast::node*>& stack) -> front::ast::node*
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
         if (auto maybe = front::lex_file(filepath, m_logger))
         {
            const auto result = fr::parse_items(maybe.value(), m_logger);
            if (result.value == fr::parse_status::error)
            {
               for (auto& err : result.errors.value())
               {
                  fmt::print(fmt::emphasis::bold, "{}:{}.{} - ", filepath.c_str(), err.pos.line,
                             err.pos.column);
                  fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "[{}]", err.type);

                  if (err.type == front::parse_error_type::e_syntax_error)
                  {
                     fmt::print(" expected '{}'\n", err.lexeme);
                  }

                  fmt::print("{}\n", err.line);
               }
            }

            write_ast_to_file(filepath, result.ast);
            write_derivations_to_file(filepath, result.derivation);

            front::ast::symbol_table_visitor st_visitor{};
            result.ast->accept(st_visitor);

            write_symbol_tables_to_file(filepath, st_visitor.get_root_table());

            for (const auto& err : st_visitor.get_errors())
            {
               fmt::print(fmt::emphasis::bold, "{}:{}.{} - ", filepath.c_str(), err.pos.line,
                          err.pos.column);
               if (err.type == front::parse_error_type::e_semantic_error)
               {
                  fmt::print(fmt::fg(fmt::color::red) | fmt::emphasis::bold, "[{}] ", err.type);
               }
               else
               {
                  fmt::print(fmt::fg(fmt::color::yellow) | fmt::emphasis::bold, "[{}] ", err.type);
               }

               fmt::print("{}\n", err.lexeme);

               fmt::print("{}\n", err.line);
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

void application::write_derivations_to_file(const std::filesystem::path& path,
                                            const std::string& derivation) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outderivation";

   std::ofstream output_file{output_path};
   fmt::print(output_file, "{}", derivation);
}

void pre_order_traversal(const front::ast::node_ptr& node, std::size_t depth,
                         std::ofstream& output_file)
{
   if (!node)
   {
      return;
   }

   const auto name = fmt::format("- {}\n", node);
   fmt::print(output_file, "{:>{}}", name, std::size(name) + depth * 2);
   // fmt::print("{:>{}}", name, std::size(name) + depth * 2);

   for (const auto& child : node->children())
   {
      pre_order_traversal(child, depth + 1, output_file);
   }

   if (std::empty(node->children()))
   {
      pre_order_traversal({}, depth + 1, output_file);
   }
}

void application::write_ast_to_file(const std::filesystem::path& path,
                                    const front::ast::node_ptr& root) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outast";

   std::ofstream output_file{output_path};

   pre_order_traversal(root, 0, output_file);
}

void st_pre_order_traversal(const front::ast::symbol_table* root, std::ofstream& output_file)
{
   if (!root)
   {
      return;
   }

   fmt::print(output_file, "{}\n\n", *root);

   for (const auto& symbol : root->symbols() | ranges::views::values)
   {
      st_pre_order_traversal(symbol.link(), output_file);
   }
}

void application::write_symbol_tables_to_file(const std::filesystem::path& path,
                                              const front::ast::symbol_table* root) const
{
   auto output_path = path.parent_path();
   output_path /= path.stem();
   output_path += ".outsymboltables";

   std::ofstream output_file{output_path};

   st_pre_order_traversal(root, output_file);
}

auto application::fancy_lexical_error_type(front::sem::token_type value) const -> std::string
{
   // NOLINTNEXTLINE
   assert(front::sem::is_token_invalid(value) && "Only invalid types should be passed");

   using namespace front::sem;

   if (value == token_type::e_invalid_char)
   {
      return "invalid character";
   }

   if (value == token_type::e_invalid_id)
   {
      return "invalid identifier";
   }

   if (value == token_type::e_invalid_num)
   {
      return "invalid number literal";
   }

   if (value == token_type::e_invalid_str)
   {
      return "invalid string literal";
   }

   return "invalid comment";
}
