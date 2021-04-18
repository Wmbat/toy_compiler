#pragma once

#include <magic_enum.hpp>

#include <fmt/core.h>

#include <cstdint>
#include <optional>
#include <vector>

namespace munster
{
   enum struct register_type : std::uint8_t
   {
      e_r0,
      e_r1,
      e_r2,
      e_r3,
      e_r4,
      e_r5,
      e_r6,
      e_r7,
      e_r8,
      e_r9,
      e_r10,
      e_r11,
      e_r12,
      e_r13,
      e_r14,
      e_r15,
   };

   class register_pool
   {
   public:
      auto pop() -> std::optional<register_type>;
      auto borrow_front() -> std::optional<register_type>;
      void push(register_type r);

   private:
      std::vector<register_type> m_available_registers{
         register_type::e_r1, register_type::e_r2,  register_type::e_r3,  register_type::e_r4,
         register_type::e_r5, register_type::e_r6,  register_type::e_r7,  register_type::e_r8,
         register_type::e_r9, register_type::e_r10, register_type::e_r11, register_type::e_r12,
         register_type::e_r13};
   };
} // namespace munster

template <>
struct fmt::formatter<munster::register_type>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(munster::register_type s, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(s).substr(2));
   }
};
