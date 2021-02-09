#pragma once

#include <filesystem>
#include <toy_compiler/lex/lexer.hpp>
#include <toy_compiler/util/logger.hpp>

#include <span>

class application
{
public:
   application(std::span<const std::string_view> args);

private:
   void write_tokens_to_file(const std::filesystem::path& path,
                             std::span<const lex::token> tokens) const;
   void write_errors_to_file(const std::filesystem::path& path,
                             std::span<const lex::token> tokens) const;

private:
   util::logger m_logger;
};
