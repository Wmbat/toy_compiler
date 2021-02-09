#include <toy_compiler/lex/lexer.hpp>

#include <toy_compiler/lex/utility.hpp>

#include <range/v3/algorithm/find.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/take_while.hpp>

namespace vi = ranges::views;

namespace lex
{
   auto handle_leading_zero(const std::string_view, std::uint32_t) -> token;
   auto handle_leading_nonzero(const std::string_view, std::uint32_t) -> token;
   auto handle_fraction(const std::string_view, std::uint32_t) -> token;
   auto handle_fraction_leading_zero(const std::string_view, std::uint32_t) -> token;
   auto handle_fraction_nonzero(const std::string_view, std::uint32_t) -> token;

   auto tokenize_numeric(const std::string_view data, std::uint32_t line) -> token
   {
      // NOLINTNEXTLINE
      assert(std::isdigit(data.at(0)) && "first character must be a digit in the range [0, 9]");

      if (data.at(0) == '0')
      {
         return handle_leading_zero(data, line);
      }

      return handle_leading_nonzero(data, line);
   }

   auto handle_leading_zero(const std::string_view data, std::uint32_t line) -> token
   {
      const char first = data.at(0);
      if (std::size(data) == 1)
      {
         return {.tok = to_string(token_type::integer_lit), .lexeme = {first}, .line = line};
      }

      const char second = data.at(1);
      if (is_alphanum(second))
      {
         const auto lexeme = data | vi::take_while(is_alphanum) | ranges::to<std::string>;
         const auto* const last = std::begin(data) + std::size(lexeme);

         if (last != std::end(data) && *last == grammar::period)
         {
            const auto float_token = handle_fraction({last, std::end(data)}, line);

            return {.tok = to_string(token_type::invalid_num),
                    .lexeme = lexeme + float_token.lexeme,
                    .line = float_token.line};
         }

         return {.tok = to_string(token_type::invalid_num), .lexeme = lexeme, .line = line};
      }

      if (second == grammar::period)
      {
         const auto float_token = handle_fraction(data.substr(1), line);
         // clang-format off
         return {
            .tok = float_token.tok, 
            .lexeme = first + float_token.lexeme, 
            .line = float_token.line
         };
         // clang-format on
      }

      return {.tok = to_string(token_type::integer_lit), .lexeme = {first}, .line = line};
   }
   auto handle_leading_nonzero(const std::string_view data, std::uint32_t line) -> token
   {
      const auto lexeme = data | vi::take_while(is_digit) | ranges::to<std::string>;
      const auto* const last = std::begin(data) + std::size(lexeme);

      if (last == std::end(data))
      {
         return {.tok = to_string(token_type::integer_lit), .lexeme = lexeme, .line = line};
      }

      if (is_alphanum(*last))
      {
         // clang-format off
         const auto error_lex = data 
            | vi::take_while([](char c) { return is_digit(c) || is_alphanum(c); }) 
            | ranges::to<std::string>;
         // clang-format on

         return {.tok = to_string(token_type::invalid_num), .lexeme = error_lex, .line = line};
         // handle error
      }

      if (*last == grammar::period)
      {
         auto float_token = handle_fraction(data.substr(std::size(lexeme)), line);

         // clang-format off
         return {
            .tok = float_token.tok, 
            .lexeme = lexeme + float_token.lexeme, 
            .line = float_token.line
         };
         // clang-format on
      }

      return {.tok = to_string(token_type::integer_lit), .lexeme = lexeme, .line = line};
   }
   auto handle_fraction(const std::string_view data, std::uint32_t line) -> token
   {
      const char period = *std::begin(data);
      const auto* const first = std::begin(data) + 1;

      if (std::size(data) == 1) // if nothing after period
      {
         return {.tok = to_string(token_type::invalid_num), .lexeme = {period}, .line = line};
      }

      if (*first == '0')
      {
         return handle_fraction_leading_zero(data.substr(1), line);
      }

      if (is_digit(*first))
      {
         return handle_fraction_nonzero(data.substr(1), line);
      }

      return {.tok = to_string(token_type::invalid_num), .lexeme = {period}, .line = line};
   }
   auto handle_fraction_leading_zero(const std::string_view data, std::uint32_t line) -> token
   {
      if (std::size(data) == 1) // only 0
      {
         return {.tok = to_string(token_type::float_lit), .lexeme = ".0", .line = line};
      }

      const char first = *(std::begin(data) + 1);
      if (is_digit(first))
      {
         return handle_fraction_nonzero(data, line);
      }

      if (std::isalpha(first))
      {
         const auto lexeme = data | vi::take_while(is_alphanum) | ranges::to<std::string>;

         return {.tok = to_string(token_type::invalid_num), .lexeme = "." + lexeme, .line = line};
      }

      return {.tok = to_string(token_type::float_lit), .lexeme = ".0", .line = line};
   }
   auto handle_fraction_nonzero(const std::string_view data, std::uint32_t line) -> token
   {
      const auto lexeme = data | vi::take_while(is_digit) | ranges::to<std::string>;

      const auto* lexeme_end = std::begin(data) + std::size(lexeme);
      if (lexeme_end != std::end(data) && std::isalpha(*lexeme_end))
      {
         const std::string_view leftovers = {lexeme_end, std::end(data)};
         const auto extra = leftovers | vi::take_while(is_alphanum) | ranges::to<std::string>;

         return {.tok = to_string(token_type::invalid_num),
                 .lexeme = "." + lexeme + extra,
                 .line = line};
      }

      const auto last = std::end(lexeme) - 1;
      if (*last == '0')
      {
         return {.tok = to_string(token_type::invalid_num), .lexeme = "." + lexeme, .line = line};
      }

      return {.tok = to_string(token_type::float_lit), .lexeme = "." + lexeme, .line = line};
   }
} // namespace lex
