#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class epsilon_node : public node
   {
   public:
      epsilon_node();

      [[nodiscard]] auto to_string() const -> std::string override;

   private:
      std::uint32_t m_index = 0;
   };
} // namespace fr::ast
