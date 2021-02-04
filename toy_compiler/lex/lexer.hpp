#pragma once

#include <toy_compiler/lex/token.hpp>
#include <toy_compiler/util/logger.hpp>

#include <libcaramel/containers/dynamic_array.hpp>

#include <monads/maybe.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace detail
{
   auto is_alphanum(char c) noexcept -> bool;
   auto is_digit(char c) noexcept -> bool;
} // namespace detail

/**
 * @brief helper class used to tokenize files based on a hand-written tokenization algorithm.
 */
class lexer
{
public:
   lexer() = default;
   lexer(util::logger_wrapper log);

   /**
    * @brief Tries to open and tokenize a file
    *
    * @param path The file to tokenize
    *
    * @return An array of tokens if the file was successfully opened, otherwise nothing
    */
   auto tokenize_file(const fs::path& path) -> monad::maybe<crl::dynamic_array<token>>;

private:
   auto next_token(const std::string_view data, std::uint32_t line_index) const -> token;

   auto create_alphanumeric_lexeme(const std::string_view data, std::uint32_t line_index) const
      -> token;
   auto handle_valid_id(const std::string_view data, std::uint32_t line_index) const -> token;
   auto handle_leading_underscore(const std::string_view data, std::uint32_t line_index) const
      -> token;

   auto create_digit_lexeme(const std::string_view data, std::uint32_t line_index) const -> token;
   auto handle_leading_zero(const std::string_view data, std::uint32_t line) const -> token;
   auto handle_regular_digit(const std::string_view data, std::uint32_t line) const -> token;

   auto trim_leading_whitespaces(const std::string_view data) const -> std::string_view;

private:
   util::logger_wrapper m_log;
};
