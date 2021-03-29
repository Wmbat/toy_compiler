#include <toy_compiler/front_end/ast_bis/factor.hpp>

#include <toy_compiler/front_end/ast_bis/operator.hpp>

namespace front::ast
{
   factor_decl::factor_decl(const source_location& location) : decl{location} {}
   factor_decl::factor_decl(const std::string& lexeme, const source_location& location) :
      decl{lexeme, location}
   {}

   integer_factor_decl::integer_factor_decl(const std::string& lexeme,
                                            const source_location& location) :
      factor_decl{lexeme, location}
   {}

   float_factor_decl::float_factor_decl(const std::string& lexeme,
                                        const source_location& location) :
      factor_decl{lexeme, location}
   {}

   str_factor_decl::str_factor_decl(const std::string& lexeme, const source_location& location) :
      factor_decl{lexeme, location}
   {}

   expr_factor_decl::expr_factor_decl(node_ptr location, node_ptr expr) :
      factor_decl{location->location()}
   {
      assert(dynamic_cast<location_decl*>(location.get())); // NOLINT
      // NOLINTNEXTLINE
      assert(dynamic_cast<factor_decl*>(expr.get()) || dynamic_cast<op*>(expr.get()));

      make_child(std::move(expr));
   }

   not_factor_decl::not_factor_decl(node_ptr value, node_ptr factor_in) :
      factor_decl{std::string{value->lexeme()}, value->location()}
   {
      assert(dynamic_cast<id_decl*>(value.get())); // NOLINT
      // NOLINTNEXTLINE
      assert(dynamic_cast<factor_decl*>(factor_in.get()) ||
             dynamic_cast<mult_op*>(factor_in.get()));

      make_child(std::move(factor_in));
   }

   sign_factor_decl::sign_factor_decl(node_ptr sign, node_ptr factor_in) :
      factor_decl{std::string{sign->lexeme()}, sign->location()}
   {
      make_child(std::move(factor_in));
   }

   auto integer_factor_decl::to_string() const -> std::string
   {
      return fmt::format("integer_factor <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto float_factor_decl::to_string() const -> std::string
   {
      return fmt::format("float_factor <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto str_factor_decl::to_string() const -> std::string
   {
      return fmt::format("str_factor <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto expr_factor_decl::to_string() const -> std::string
   {
      return fmt::format("expr_factor <line:{1}, col:{2}>", lexeme(), location().line,
                         location().column);
   }
   auto not_factor_decl::to_string() const -> std::string
   {
      return fmt::format("not_factor <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
   auto sign_factor_decl::to_string() const -> std::string
   {
      return fmt::format("not_factor <line:{1}, col:{2}> '{0}'", lexeme(), location().line,
                         location().column);
   }
} // namespace front::ast
