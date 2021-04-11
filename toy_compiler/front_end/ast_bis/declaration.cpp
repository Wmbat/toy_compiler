#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <toy_compiler/munster/ast/decl/compound_func_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_func_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_var_decl.hpp>
#include <toy_compiler/munster/ast/stmt/compound_stmt.hpp>

#include <toy_compiler/munster/visitor/visitor.hpp>

#include <cassert>
#include <utility>

namespace munster::ast
{
   visibility_decl::visibility_decl(const std::string& name,
                                    const front::source_location& location) :
      decl{name, location}
   {}

   auto visibility_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "visibility_decl", location().line,
                         location().column, lexeme());
   }

   variable_decl::variable_decl(node_ptr type, node_ptr id, node_ptr compound_array_decl) :
      decl{std::string{id->lexeme()}, type->location()},
      m_type(type->lexeme())
   {
      if (!std::empty(compound_array_decl->children()))
      {
         make_child(std::move(compound_array_decl));
      }
   }

   auto variable_decl::type() const -> std::string_view { return m_type; }
   auto variable_decl::to_string() const -> std::string
   {
      return fmt::format("variable_decl <line:{}, col:{}> {} '{}'", location().line,
                         location().column, lexeme(), m_type);
   }

   compound_array_decl::compound_array_decl(std::vector<node_ptr>&& array_decls)
   {
      make_family<array_decl>(std::move(array_decls));
   }

   auto compound_array_decl::to_string() const -> std::string { return "compound_array_decl"; }

   auto get_lexeme(node_ptr& node) -> std::string
   {
      if (node)
      {
         return std::string{node->lexeme()};
      }

      return {};
   }

   array_decl::array_decl(node_ptr start_loc, node_ptr integer_lit, node_ptr end_loc) :
      decl{get_lexeme(integer_lit), start_loc->location()},
      m_end_loc{end_loc->location()}
   {}

   auto array_decl::to_string() const -> std::string
   {
      return fmt::format("array_decl <line:{}, col:{}> <line:{}, col:{}> '[{}]'", location().line,
                         location().column, m_end_loc.line, m_end_loc.column, lexeme());
   }

   compound_params_decl::compound_params_decl(std::vector<node_ptr>&& param_decls)
   {
      make_family<variable_decl>(std::move(param_decls));
   }

   auto compound_params_decl::to_string() const -> std::string { return "compound_param_decl"; }

   compound_variable_decl::compound_variable_decl(std::vector<node_ptr>&& variables)
   {
      make_family<variable_decl>(std::move(variables));
   }

   void compound_variable_decl::accept(visitor_variant&) const {}

   auto compound_variable_decl::to_string() const -> std::string
   {
      return fmt::format("compound_variable_decl");
   }

   main_decl::main_decl(node_ptr id, node_ptr func_body) :
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

   compound_array_index_access_decl::compound_array_index_access_decl(
      std::vector<node_ptr>&& variables)
   {
      make_family<array_index_access_decl>(std::move(variables));
   }

   array_index_access_decl::array_index_access_decl(node_ptr beg, node_ptr expr, node_ptr end) :
      decl{beg->location()},
      m_end{end->location()}
   {
      /*
      assert(dynamic_cast<location_decl*>(beg.get())); // NOLINT
      assert(dynamic_cast<location_decl*>(end.get())); // NOLINT
      */

      make_child(std::move(expr));
   }

   auto compound_array_index_access_decl::to_string() const -> std::string
   {
      return "compound_array_index_access_decl";
   }

   auto array_index_access_decl::to_string() const -> std::string
   {
      return fmt::format("array_index_access_decl <line:{}, col:{}> <line:{}, col:{}>",
                         location().line, location().column, m_end.line, m_end.column);
   }

   stmt_block_decl::stmt_block_decl(node_ptr node)
   {
      if (node)
      {
         make_child(std::move(node));
      }
   }

   auto stmt_block_decl::to_string() const -> std::string { return "stmt_block_decl"; }

}; // namespace munster::ast
