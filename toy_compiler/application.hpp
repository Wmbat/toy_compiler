#pragma once

#include <toy_compiler/lexer.hpp>

#include <span>

class application
{
public:
   application(std::span<const std::string_view> args);

private:
   lexer m_lex{};
};
