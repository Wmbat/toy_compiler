#include <toy_compiler/front_end/ast/type.hpp>

namespace fr::ast
{
   static inline std::uint32_t type_counter = 0; // NOLINT

   type::type(std::string type_name, const source_location& location) :
      node(std::move(type_name), location),
      m_index{type_counter++}
   {}

   auto type::to_string() const -> std::string
   {
      const auto name =
         fmt::format("\"{}_{}\"", sem::to_string_view(sem::action_type::type), m_index);
      return fmt::format("{0};\n{0} [label=\"{1}\"]\n", name, lexeme().value());
   }

   integer_type::integer_type(const source_location& location) : type("integer", location) {}
   float_type::float_type(const source_location& location) : type("float", location) {}
   string_type::string_type(const source_location& location) : type("string", location) {}
   user_defined_type::user_defined_type(std::string type_name, const source_location& location) :
      type(std::move(type_name), location)
   {}
} // namespace fr::ast
