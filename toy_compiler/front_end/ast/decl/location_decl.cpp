#include <toy_compiler/front_end/ast/decl/location_decl.hpp>

namespace front::ast
{
   location_decl::location_decl(const source_location& location) : decl{location} {}

   auto location_decl::to_string() const -> std::string { return fmt::format("{}", location()); }
} // namespace front::ast
