/**
 * @file application.hpp
 * @brief Contains the primary entry point for the compiler and all the compiling facilities
 */

#pragma once

#include <toy_compiler/lex/lexer.hpp>
#include <toy_compiler/util/logger.hpp>

#include <filesystem>
#include <span>

/**
 * @brief The main entry point for the compiler
 */
class application
{
public:
   application(std::span<const std::string_view> args, util::logger_wrapper log = nullptr);

private:
   void write_tokens_to_file(const std::filesystem::path& path, std::span<const lex::item> tokens,
                             util::logger_wrapper log) const;
   void write_errors_to_file(const std::filesystem::path& path, std::span<const lex::item> tokens,
                             util::logger_wrapper log) const;

   [[nodiscard]] auto fancy_lexical_error_type(const std::string_view type) const -> std::string;

private:
   util::logger_wrapper m_logger;
};
