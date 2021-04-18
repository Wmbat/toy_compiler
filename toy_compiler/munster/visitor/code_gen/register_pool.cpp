#include <toy_compiler/munster/visitor/code_gen/register_pool.hpp>

#include <range/v3/algorithm/find.hpp>

namespace munster
{
   auto register_pool::pop() -> std::optional<register_type>
   {
      if (!std::empty(m_available_registers))
      {
         const auto temp = m_available_registers.back();
         m_available_registers.pop_back();

         return temp;
      }

      return std::nullopt;
   }
   auto register_pool::borrow_front() -> std::optional<register_type>
   {
      if (!std::empty(m_available_registers))
      {
         return m_available_registers.front();
      }

      return std::nullopt;
   }
   void register_pool::push(register_type r)
   {
      const auto it = ranges::find(m_available_registers, r);
      if (it == std::end(m_available_registers))
      {
         m_available_registers.push_back(r);
      }
   }
} // namespace munster
