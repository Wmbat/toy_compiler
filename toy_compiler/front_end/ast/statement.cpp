#include <toy_compiler/front_end/ast/statement.hpp>

namespace fr::ast
{
   static inline std::uint32_t counter = 0; // NOLINT

   statement::statement(const source_location& location) : node{{}, location, counter++} {}

   continue_statement::continue_statement(const source_location& location) : statement{location} {}

   auto continue_statement::to_string() const -> std::string
   {
      const auto continue_name = magic_enum::enum_name(grammar::token_type::id_continue);
      const auto name = fmt::format("\"{}_{}\"", continue_name, index());
      const auto label = fmt::format("{} [label={}]\n", name, continue_name);

      return fmt::format("{}\n{}", name, label);
   }

   break_statement::break_statement(const source_location& location) : statement{location} {}

   auto break_statement::to_string() const -> std::string
   {
      const auto continue_name = magic_enum::enum_name(grammar::token_type::id_break);
      const auto name = fmt::format("\"{}_{}\"", continue_name, index());
      const auto label = fmt::format("{} [label={}]\n", name, continue_name);

      return fmt::format("{}\n{}", name, label);
   }

   read_statement::read_statement(node_ptr variable, const source_location& location) :
      statement{location}
   {
      make_child(std::move(variable));
   }

   auto read_statement::to_string() const -> std::string
   {
      const auto read_name = magic_enum::enum_name(grammar::token_type::id_read);
      const auto name = fmt::format("\"{}_{}\"", read_name, index());
      auto output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, read_name);

      return fmt::format("{};\n{}", name, output);
   }

   return_statement::return_statement(node_ptr expr, const source_location& location) :
      statement{location}
   {
      make_child(std::move(expr));
   }

   auto return_statement::to_string() const -> std::string
   {
      const auto read_name = magic_enum::enum_name(grammar::token_type::id_return);
      const auto name = fmt::format("\"{}_{}\"", read_name, index());
      auto output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, read_name);

      return fmt::format("{};\n{}", name, output);
   }

   write_statement::write_statement(node_ptr expr, const source_location& location) :
      statement{location}
   {
      make_child(std::move(expr));
   }

   auto write_statement::to_string() const -> std::string
   {
      const auto read_name = magic_enum::enum_name(grammar::token_type::id_return);
      const auto name = fmt::format("\"{}_{}\"", read_name, index());
      auto output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, read_name);

      return fmt::format("{};\n{}", name, output);
   }

   while_statement::while_statement(node_ptr expr, node_ptr stat_block,
                                    const source_location& location) :
      statement{location}
   {
      expr->make_sibling(std::move(stat_block));
      make_child(std::move(expr));
   }

   auto while_statement::to_string() const -> std::string
   {
      const auto read_name = magic_enum::enum_name(grammar::token_type::id_while);
      const auto name = fmt::format("\"{}_{}\"", read_name, index());
      auto output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, read_name);

      return fmt::format("{};\n{}", name, output);
   }

   if_statement::if_statement(node_ptr expr, node_ptr stat_block_then, node_ptr stat_block_else,
                              const source_location& location) :
      statement{location}
   {
      stat_block_then->make_sibling(std::move(stat_block_else));
      expr->make_sibling(std::move(stat_block_then));
      make_child(std::move(expr));
   }

   auto if_statement::to_string() const -> std::string
   {
      const auto read_name = magic_enum::enum_name(grammar::token_type::id_if);
      const auto name = fmt::format("\"{}_{}\"", read_name, index());
      auto output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, read_name);

      return fmt::format("{};\n{}", name, output);
   }

   statement_block::statement_block(node_ptr node, const source_location& location) :
      statement{location}
   {
      make_child(std::move(node));
   }

   auto statement_block::to_string() const -> std::string
   {
      std::string name = fmt::format("\"{}_{}\"", "statement_block", index());
      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, "statement_block");

      return fmt::format("{};\n{}", name, output);
   }
} // namespace fr::ast
