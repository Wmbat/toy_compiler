#pragma once

#include <toy_compiler/munster/source_location.hpp>

#include <magic_enum.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <memory>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>

namespace munster
{
   enum struct symbol_type
   {
      e_undefined,
      e_main,
      e_class,
      e_inheritance,
      e_member_variable,
      e_member_function,
      e_method,
      e_function,
      e_variable,
      e_parameter,
      e_temporary
   };

   enum struct symbol_table_type
   {
      e_undefined,
      e_translation_unit,
      e_class,
      e_func,
      e_main
   };

   class symbol_table;

   struct symbol_create_info
   {
      std::string name;
      symbol_type kind;
      source_location location;

      std::int64_t size{0};
      std::string type{""};
      std::unique_ptr<symbol_table> link{};
   };

   class symbol
   {
   public:
      symbol();
      symbol(symbol_create_info&& info);

      auto name() const noexcept -> std::string_view;           // NOLINT
      auto type() const noexcept -> std::string_view;           // NOLINT
      auto kind() const noexcept -> symbol_type;                // NOLINT
      auto size() const noexcept -> std::int64_t;               // NOLINT
      auto link() const noexcept -> symbol_table*;              // NOLINT
      auto location() const noexcept -> const source_location&; // NOLINT

      void update_size(std::int64_t size);
      void set_table(std::unique_ptr<symbol_table> table);

   private:
      std::string m_name{};
      std::string m_type{};
      symbol_type m_kind{};
      std::int64_t m_size{};

      std::unique_ptr<symbol_table> m_link;

      source_location m_location;
   };

   class symbol_table
   {
      using container = std::unordered_map<std::string, symbol>;

   public:
      using key_type = std::string;
      using mapped_type = symbol;
      using iterator = container::iterator;
      using const_iterator = container::const_iterator;

      class insert_kv_result
      {
      public:
         insert_kv_result(iterator m_value, bool insertion);

         auto is_inserted() const noexcept -> bool; // NOLINT

         auto key() const -> std::string_view; // NOLINT
         auto val() const -> symbol&;          // NOLINT

      private:
         bool m_insertion;
         iterator m_value;
      };

      class lookup_kv_result
      {
      public:
         lookup_kv_result(const std::string* p_key, symbol* p_val);

         explicit operator bool() const;

         auto key() const -> std::string_view; // NOLINT
         auto val() const -> symbol&;          // NOLINT

      private:
         const std::string* m_key;
         symbol* m_value;
      };

      class remove_kv_result
      {
      public:
         remove_kv_result() = default;
         remove_kv_result(std::string&& key, symbol&& val);

         explicit operator bool() const;

         auto key() const -> std::string_view; // NOLINT
         auto val() const -> const symbol&;    // NOLINT

         auto take_key() -> std::string;
         auto take_val() -> symbol;

      private:
         std::optional<std::string> m_key = std::nullopt;
         std::optional<symbol> m_val = std::nullopt;
      };

   public:
      symbol_table() = default;
      symbol_table(std::string name, symbol_table_type kind);

      auto name() const noexcept -> std::string_view;  // NOLINT
      auto kind() const noexcept -> symbol_table_type; // NOLINT
      auto symbols() const -> const std::unordered_map<std::string, symbol>&;

      auto insert(const std::string& name, symbol&& value) -> insert_kv_result;
      auto lookup(const std::string& name) -> lookup_kv_result;
      auto remove(const std::string& name) -> remove_kv_result;

   private:
      std::string m_name = "default";
      symbol_table_type m_kind{symbol_table_type::e_undefined};

      std::unordered_map<std::string, symbol> m_symbols;
   };
} // namespace munster

template <>
struct fmt::formatter<munster::symbol_type>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(munster::symbol_type s, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(s).substr(2));
   }
};

template <>
struct fmt::formatter<munster::symbol>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const munster::symbol& s, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "symbol(name={}, kind={}, type={}, link={})", s.name(),
                            s.kind(), s.type(), s.link() ? "Yes" : "No");
   }
};

template <>
struct fmt::formatter<munster::symbol_table>
{
   template <typename ParseContex>
   constexpr auto parse(ParseContex& ctx)
   {
      return ctx.begin();
   }

   template <typename FormatContext>
   auto format(const munster::symbol_table& s, FormatContext& ctx)
   {
      return fmt::format_to(ctx.out(), "symbol_table(\n\tname={}, \n\tkind={}, \n\tvalues={})",
                            s.name(), magic_enum::enum_name(s.kind()).substr(2),
                            fmt::join(s.symbols() | ranges::views::values, "\n\t       "));
   }
};
