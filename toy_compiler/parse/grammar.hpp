#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace parse
{
   enum struct symbol_type
   {
      terminal,
      non_terminal,
      start,
      stop
   };

   class symbol
   {
   public:
      static constexpr symbol_type terminal = symbol_type::terminal;
      static constexpr symbol_type non_terminal = symbol_type::non_terminal;
      static constexpr symbol_type start = symbol_type::start;
      static constexpr symbol_type stop = symbol_type::stop;

   public:
      symbol();

      friend auto operator==(const symbol& symbol, symbol_type type) -> bool;

   private:
      std::uint32_t value;
   };

   // terminal = lex::item_type
   // non_terminal =
   //

   class rule
   {
   };
} // namespace parse
