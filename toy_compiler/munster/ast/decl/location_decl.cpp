#include <toy_compiler/munster/ast/decl/location_decl.hpp>

namespace munster::ast
{
   location_decl::location_decl(const source_location& location) : decl{location} {}

   auto location_decl::to_string() const -> std::string { return fmt::format("{}", location()); }
} // namespace munster::ast
