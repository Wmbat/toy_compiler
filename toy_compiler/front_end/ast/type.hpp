#pragma once

#include <toy_compiler/front_end/ast/node.hpp>

namespace fr::ast
{
   class type : public node
   {
   public:
      type(std::string type_name, const source_location& location);

      [[nodiscard]] auto to_string() const -> std::string final;

   private:
      std::uint32_t m_index = 0;
   };

   class integer_type : public type
   {
   public:
      integer_type(const source_location& location);
   };

   class float_type : public type
   {
   public:
      float_type(const source_location& location);
   };

   class string_type : public type
   {
   public:
      string_type(const source_location& location);
   };

   class user_defined_type : public type
   {
   public:
      user_defined_type(std::string type_name, const source_location& location);
   };
} // namespace fr::ast
