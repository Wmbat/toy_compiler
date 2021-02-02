#include <toy_compiler/application.hpp>

application::application(std::span<const std::string_view> args)
{
   m_lex.tokenize_file(args[0]);
}
