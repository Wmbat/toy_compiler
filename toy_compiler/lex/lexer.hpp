#pragma once

#include <toy_compiler/lex/token.hpp>
#include <toy_compiler/util/logger.hpp>

#include <libcaramel/containers/dynamic_array.hpp>

#include <monads/maybe.hpp>

#include <filesystem>

namespace lex
{
   auto tokenize_file(const std::filesystem::path& path, util::logger_wrapper log = nullptr)
      -> monad::maybe<crl::dynamic_array<token>>;

   auto tokenize_alphanum(const std::string_view data, std::uint32_t line) -> token;
   auto tokenize_braces(const std::string_view data, std::uint32_t line) -> token;
   auto tokenize_comments(const std::string_view data, std::uint32_t line) -> token;
   auto tokenize_numeric(const std::string_view data, std::uint32_t line) -> token;
   auto tokenize_punctuation(const std::string_view data, std::uint32_t line) -> token;
   auto tokenize_string(const std::string_view data, std::uint32_t line) -> token;
} // namespace lex
