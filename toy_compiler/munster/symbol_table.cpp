#include <string_view>

#include <toy_compiler/munster/symbol_table.hpp>

#include <utility>

#include <range/v3/algorithm/find.hpp>

namespace munster
{

   symbol::symbol(symbol_create_info&& info) :
      m_name{std::move(info.name)},
      m_type{std::move(info.type)},
      m_kind{info.kind},
      m_size{info.size},
      m_link{std::move(info.link)},
      m_location{info.location}
   {}

   auto symbol::name() const noexcept -> std::string_view { return m_name; }
   auto symbol::type() const noexcept -> std::string_view { return m_type; }
   auto symbol::kind() const noexcept -> symbol_type { return m_kind; }
   auto symbol::link() const noexcept -> symbol_table* { return m_link.get(); }
   auto symbol::location() const noexcept -> const source_location& { return m_location; }

   void symbol::update_size(std::int64_t size) { m_size = size; }
   void symbol::set_table(std::unique_ptr<symbol_table> table) { m_link = std::move(table); }

   symbol_table::lookup_kv_result::lookup_kv_result(const std::string* p_key, symbol* p_val) :
      m_key{p_key},
      m_value{p_val}
   {}

   symbol_table::lookup_kv_result::operator bool() const { return m_key != nullptr; }

   auto symbol_table::lookup_kv_result::key() const -> std::string_view { return *m_key; }
   auto symbol_table::lookup_kv_result::val() const -> symbol& { return *m_value; }

   symbol_table::insert_kv_result::insert_kv_result(iterator value, bool insertion) :
      m_insertion{insertion},
      m_value{value}
   {}

   auto symbol_table::insert_kv_result::is_inserted() const noexcept -> bool { return m_insertion; }

   auto symbol_table::insert_kv_result::key() const -> std::string_view { return m_value->first; }
   auto symbol_table::insert_kv_result::val() const -> symbol& { return m_value->second; }

   symbol_table::remove_kv_result::remove_kv_result(std::string&& key, symbol&& val) :
      m_key{std::move(key)},
      m_val{std::move(val)}
   {}

   symbol_table::remove_kv_result::operator bool() const { return m_key.has_value(); }

   auto symbol_table::remove_kv_result::key() const -> std::string_view { return m_key.value(); }
   auto symbol_table::remove_kv_result::val() const -> const symbol& { return m_val.value(); }

   auto symbol_table::remove_kv_result::take_key() -> std::string
   {
      return std::move(m_key.value());
   }
   auto symbol_table::remove_kv_result::take_val() -> symbol { return std::move(m_val.value()); }

   symbol_table::symbol_table(std::string name, symbol_table_type kind) :
      m_name{std::move(name)},
      m_kind{kind}
   {}

   auto symbol_table::name() const noexcept -> std::string_view { return m_name; }
   auto symbol_table::kind() const noexcept -> symbol_table_type { return m_kind; }
   auto symbol_table::symbols() const -> const std::unordered_map<std::string, symbol>&
   {
      return m_symbols;
   }

   auto symbol_table::lookup(const std::string& name) -> lookup_kv_result
   {
      auto it = m_symbols.find(name);
      if (it != std::end(m_symbols))
      {
         const std::string* p_key = &(it->first);
         symbol* p_val = &(it->second);

         return lookup_kv_result{p_key, p_val};
      }
      else
      {
         return lookup_kv_result{nullptr, nullptr};
      }
   }
   auto symbol_table::insert(const std::string& name, symbol&& value) -> insert_kv_result
   {
      auto [it, insertion] = m_symbols.insert({name, std::move(value)});

      return {it, insertion};
   }
   auto symbol_table::remove(const std::string& name) -> remove_kv_result
   {
      auto it = m_symbols.find(name);

      if (it != std::end(m_symbols))
      {
         auto key = it->first;
         auto val = std::move(it->second);

         m_symbols.erase(it);

         return {std::move(key), std::move(val)};
      }

      return {};
   }

} // namespace munster
