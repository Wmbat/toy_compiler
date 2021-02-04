#include <toy_compiler/core/application.hpp>

#include <iostream>

application::application(std::span<const std::string_view> args) :
   m_logger{"toy compiler"},
   m_lex{&m_logger}
{
   for (auto filename : args)
   {
      if ([[maybe_unused]] auto test = m_lex.tokenize_file(filename))
      {
      }
   }
}
