#include <toy_compiler/munster/ast/decl/inheritance_decl.hpp>

#include <mpark/patterns.hpp>

namespace munster::ast
{
   inheritance_decl::inheritance_decl(const std::string& name,
                                      const front::source_location& location) :
      decl{name, location}
   {}

   void inheritance_decl::accept(visitor_variant& visitor) const
   {
      using namespace mpark::patterns;

      const auto visit = [this](auto& vis) {
         vis(*this);
      };

      match(visitor)(pattern(as<symbol_table_visitor>(arg)) = visit,
                     pattern(as<type_checking_visitor>(arg)) = visit);
   }

   auto inheritance_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "inheritance_decl", location().line,
                         location().column, lexeme());
   }
} // namespace munster::ast
