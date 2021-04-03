#pragma once

#include <toy_compiler/front_end/source_location.hpp>

#include <fmt/core.h>

#include <magic_enum.hpp>

#include <string>

namespace front
{
   enum struct parse_error_type : std::uint32_t
   {
      e_syntax_error,
      e_semantic_error,
      e_semantic_warning,
      e_max_size
   };

   struct parse_error
   {
      parse_error_type type{parse_error_type::e_max_size};
      front::source_location pos{};
      std::string lexeme;
      std::string line;
   };
} // namespace front

template <>
struct fmt::formatter<front::parse_error_type>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(front::parse_error_type error_type, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(error_type).substr(2));
   }
};
