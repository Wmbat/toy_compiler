#include <toy_compiler/lex/utility.hpp>

#include <toy_compiler/lex/token.hpp>

#include <cctype>

#include <range/v3/algorithm/find.hpp>

namespace lex
{
   constexpr std::array punctuations{grammar::period, grammar::comma, grammar::colon,
                                     grammar::semi_colon};
   constexpr std::array braces{grammar::open_parenth, grammar::close_parenth, grammar::open_square,
                               grammar::close_square, grammar::open_curly,    grammar::close_curly};
   constexpr std::array operators{'=', '+', '-', '*', '/', '<', '>', '|', '&', '!', '?'};
   constexpr std::array comments{"/*", "//"};

   auto is_alphabet(char c) noexcept -> bool { return std::isalpha(c); }
   auto is_alphanum(char c) noexcept -> bool { return std::isalnum(c) || c == '_'; }
   auto is_quote(char c) noexcept -> bool { return c == '\"'; }
   auto is_digit(char c) noexcept -> bool { return std::isdigit(c); }
   auto is_operator(char c) noexcept -> bool
   {
      return ranges::find(operators, c) != std::end(operators);
   }
   auto is_punctuation(char c) noexcept -> bool
   {
      return ranges::find(punctuations, c) != std::end(punctuations);
   }
   auto is_braces(char c) noexcept -> bool { return ranges::find(braces, c) != std::end(braces); }
   auto is_newline(char c) noexcept -> bool { return c == '\n'; }
   auto is_comment(std::string_view str) -> bool
   {
      return ranges::find(comments, str) != std::end(comments);
   }

   auto to_literal(const std::string_view data) -> std::string
   {
      std::string res{data};

      {
         std::size_t start_pos = 0;
         while ((start_pos = res.find('\n', start_pos)) != std::string::npos)
         {
            res.replace(start_pos, 1, "\\n");
            start_pos += std::size("\\n");
         }
      }

      {
         std::size_t start_pos = 0;
         while ((start_pos = res.find('\r', start_pos)) != std::string::npos)
         {
            res.replace(start_pos, 1, "\\r");
            start_pos += std::size("\\r");
         }
      }

      return res;
   }
} // namespace lex
