#pragma once

#include <toy_compiler/util/crtp.hpp>

#include <toy_compiler/front_end/ast/visitor/visitor.hpp>

namespace front::ast
{
   template <typename Any>
   class element_intf : util::crtp<Any, element_intf>
   {
   public:
      void accept(visitor& visitor) const
      {
         for (const auto& node : this->underlying().children())
         {
            std::visit(
               [&](const auto& value) {
                  value.accept(visitor);
               },
               node);
         }

         visitor.visit(&this->underlying());
      }
   };

   template <typename Any>
   class end_element_intf : util::crtp<Any, element_intf>
   {
   public:
      void accept(visitor& visitor) const { visitor.visit(&this->underlying()); }
   };
} // namespace front::ast
