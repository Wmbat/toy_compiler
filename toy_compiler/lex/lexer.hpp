#pragma once

#include <toy_compiler/lex/token.hpp>
#include <toy_compiler/util/logger.hpp>

#include <libcaramel/containers/dynamic_array.hpp>

#include <monads/maybe.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace detail
{
   auto is_alphabet(char c) noexcept -> bool;
   auto is_alphanum(char c) noexcept -> bool;
   auto is_digit(char c) noexcept -> bool;
   auto is_punctuation(char c) noexcept -> bool;
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
   [[nodiscard]] auto next_token(const std::string_view data, std::uint32_t line_index) const
      -> token;

   // ID & KEYWORD

   [[nodiscard]] auto create_alphanumeric_token(const std::string_view data,
                                                std::uint32_t line) const -> token;
   [[nodiscard]] auto handle_valid_id(const std::string_view data, std::uint32_t line) const
      -> token;
   [[nodiscard]] auto handle_alpha_leading_underscore(const std::string_view data,
                                                      std::uint32_t line) const -> token;

   // FLOATS & INTS

   [[nodiscard]] auto create_digit_token(const std::string_view data,
                                         std::uint32_t line_index) const -> token;
   [[nodiscard]] auto handle_leading_zero(const std::string_view data, std::uint32_t line) const
      -> token;
   [[nodiscard]] auto handle_continued_leading_zero(const std::string_view data,
                                                    std::uint32_t line) const -> token;
   [[nodiscard]] auto handle_leading_nonzero(const std::string_view data, std::uint32_t line) const
      -> token;

   [[nodiscard]] auto handle_floats(const std::string_view data, std::uint32_t line) const -> token;
   [[nodiscard]] auto handle_float_zero(const std::string_view data, std::uint32_t line) const
      -> token;
   [[nodiscard]] auto handle_float_nonzero(const std::string_view data, std::uint32_t line) const
      -> token;

   // PUNCTUATION
   [[nodiscard]] auto create_punctuation_token(const std::string_view data,
                                               std::uint32_t line) const -> token;
   [[nodiscard]] auto handle_colon(std::string_view data, std::uint32_t line) const -> token;

   [[nodiscard]] auto trim_leading_whitespaces(const std::string_view data) const
      -> std::string_view;

private:
   util::logger_wrapper m_log;
};
