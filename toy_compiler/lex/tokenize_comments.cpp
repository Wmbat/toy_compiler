#include <toy_compiler/lex/lexer.hpp>

namespace lex
{
   auto tokenize_comments(const std::string_view /*data*/, std::uint32_t /*line*/) -> token
   {
      return {};
   }
} // namespace lex
