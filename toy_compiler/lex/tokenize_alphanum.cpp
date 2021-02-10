#include <toy_compiler/lex/lexer.hpp>

#include <toy_compiler/lex/utility.hpp>

#include <range/v3/algorithm/find.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/take_while.hpp>

namespace vi = ranges::views;

namespace lex
{
   auto handle_leading_alphabet(const std::string_view, std::uint32_t) -> token;
   auto handle_leading_underscore(const std::string_view, std::uint32_t) -> token;

   auto tokenize_alphanum(const std::string_view data, std::uint32_t line) -> token
   {
      // NOLINTNEXTLINE
      assert((std::isalpha(data.at(0)) || data.at(0) == '_') &&
             "first character must be an alphabet letter or underscore");

      if (std::isalpha(data.at(0)))
      {
         return handle_leading_alphabet(data, line);
      }

      return handle_leading_underscore(data, line);
   }

   auto handle_leading_alphabet(const std::string_view data, std::uint32_t line) -> token
   {
      const auto lexeme = data | vi::take_while(is_alphanum) | ranges::to<std::string>;

      if (const auto* it = ranges::find(keywords, lexeme); it != std::end(keywords))
      {
         return {.type = *it, .lexeme = lexeme, .line = line};
      }

      return {.type = to_string_view(token_type::id), .lexeme = lexeme, .line = line};
   }
   auto handle_leading_underscore(const std::string_view data, std::uint32_t line) -> token
   {
      const auto lexeme = data | vi::take_while(is_alphanum) | ranges::to<std::string>;

      if (std::size(lexeme) == 1)
      {
         return {.type = to_string_view(token_type::invalid_char), .lexeme = lexeme, .line = line};
      }

      return {.type = to_string_view(token_type::invalid_id), .lexeme = lexeme, .line = line};
   }
} // namespace lex
