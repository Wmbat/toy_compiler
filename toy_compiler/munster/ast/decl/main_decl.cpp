#include <toy_compiler/munster/ast/decl/main_decl.hpp>

namespace munster::ast
{
   main_decl::main_decl(id_decl::ptr id, func_body_decl::ptr func_body) :
      decl{std::string{id->lexeme()}, id->location()}
   {
      make_child(std::move(func_body));
   }

   void main_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      std::visit(
         [this](auto& visitor) {
            visitor.visit(*this);
         },
         visitor);
   }

   auto main_decl::to_string() const -> std::string
   {
      return fmt::format("main_decl <line:{}, col:{}> {}", location().line, location().column,
                         lexeme());
   }
} // namespace munster::ast
