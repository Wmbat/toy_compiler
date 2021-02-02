#pragma once

#include <toy_compiler/token.hpp>

#include <libcaramel/containers/dynamic_array.hpp>

#include <monads/maybe.hpp>

#include <filesystem>

namespace fs = std::filesystem;

namespace detail
{
   auto is_alphanum(char c) noexcept -> bool;
}

class lexer
{
   static constexpr std::size_t start_state = 0;
   static constexpr std::size_t char_buffer_size = 1024;

public:
   auto tokenize_file(const fs::path& path) -> crl::dynamic_array<token>;

private:
   auto next_token(const std::string_view data, std::uint32_t line_index) const -> token;
   auto trim_leading_whitespaces(const std::string_view data) const -> std::string_view;

   auto handle_valid_id(const std::string_view data, std::uint32_t line_index) const -> token;
   auto handle_leading_underscore(const std::string_view data, std::uint32_t line_index) const
      -> token;

private:
   crl::dynamic_array<token> m_tokens{};
};
