#pragma once

#include <toy_compiler/lex/lexer.hpp>
#include <toy_compiler/util/logger.hpp>

#include <span>

class application
{
public:
   application(std::span<const std::string_view> args);

private:
   util::logger m_logger;

   lexer m_lex{};
};
