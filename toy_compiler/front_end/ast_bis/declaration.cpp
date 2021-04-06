#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <toy_compiler/front_end/ast/node/compound_func_decl.hpp>
#include <toy_compiler/front_end/ast/node/compound_stmt.hpp>
#include <toy_compiler/front_end/ast/node/member_func_decl.hpp>
#include <toy_compiler/front_end/ast/node/member_var_decl.hpp>

#include <toy_compiler/front_end/ast/visitor/visitor.hpp>

#include <cassert>
#include <utility>

namespace front::ast
{
   decl::decl(const source_location& location) : node{location} {}
   decl::decl(const std::string& lexeme, const source_location& location) : node{lexeme, location}
   {}

   translation_unit_decl::translation_unit_decl(node_ptr compound_class, node_ptr compound_function,
                                                node_ptr main)
   {
      assert(dynamic_cast<compound_class_decl*>(compound_class.get()));   // NOLINT
      assert(dynamic_cast<compound_func_decl*>(compound_function.get())); // NOLINT
      assert(dynamic_cast<main_decl*>(main.get()));                       // NOLINT

      make_child(std::move(compound_class));
      make_child(std::move(compound_function));
      make_child(std::move(main));
   }

   auto translation_unit_decl::to_string() const -> std::string { return "translation_unit_decl"; }

   location_decl::location_decl(const source_location& location) : decl{location} {}

   auto location_decl::to_string() const -> std::string { return fmt::format("{}", location()); }

   id_decl::id_decl(const std::string& lexeme, const source_location& location) :
      decl{lexeme, location}
   {}

   auto id_decl::to_string() const -> std::string { return fmt::format("{}", lexeme()); }

   type_decl::type_decl(const std::string& name, const source_location& location) :
      decl{name, location}
   {}

   auto type_decl::to_string() const -> std::string { return fmt::format("{}", lexeme()); }

   compound_class_decl::compound_class_decl(std::vector<node_ptr>&& class_decls)
   {
      make_family<class_decl>(std::move(class_decls));
   };

   auto compound_class_decl::to_string() const -> std::string { return "compound_class_decl"; }

   class_decl::class_decl(node_ptr class_start, node_ptr class_name, node_ptr compound_inheritance,
                          node_ptr compound_member) :
      decl{class_name->to_string(), class_start->location()}
   {
      assert(dynamic_cast<location_decl*>(class_start.get()));                      // NOLINT
      assert(dynamic_cast<id_decl*>(class_name.get()));                             // NOLINT
      assert(dynamic_cast<compound_inheritance_decl*>(compound_inheritance.get())); // NOLINT
      assert(dynamic_cast<compound_member_decl*>(compound_member.get()));           // NOLINT

      if (!std::empty(compound_inheritance->children()))
      {
         make_child(std::move(compound_inheritance));
      }

      if (!std::empty(compound_member->children()))
      {
         make_child(std::move(compound_member));
      }
   }

   auto class_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "class_decl", location().line,
                         location().column, lexeme());
   }

   compound_inheritance_decl::compound_inheritance_decl(std::vector<node_ptr>&& inheritance_decls)
   {
      make_family<inheritance_decl>(std::move(inheritance_decls));
   }

   auto compound_inheritance_decl::to_string() const -> std::string
   {
      return "compound_inheritance_decl";
   }

   inheritance_decl::inheritance_decl(const std::string& name, const source_location& location) :
      decl{name, location}
   {}

   auto inheritance_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "inheritance_decl", location().line,
                         location().column, lexeme());
   }

   compound_member_decl::compound_member_decl(std::vector<node_ptr>&& member_decls)
   {
      make_family<member_func_decl, member_var_decl>(std::move(member_decls));
   }

   auto compound_member_decl::to_string() const -> std::string { return "compound_member_decl"; }

   visibility_decl::visibility_decl(const std::string& name, const source_location& location) :
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

   void compound_variable_decl::accept(visitor&) const {}

   auto compound_variable_decl::to_string() const -> std::string
   {
      return fmt::format("compound_variable_decl");
   }

   main_decl::main_decl(node_ptr id, node_ptr func_body) :
      decl{std::string{id->lexeme()}, id->location()}
   {
      make_child(std::move(func_body));
   }

   void main_decl::accept(visitor& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visitor.visit(this);
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
      assert(dynamic_cast<location_decl*>(beg.get())); // NOLINT
      assert(dynamic_cast<location_decl*>(end.get())); // NOLINT

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

   void decl::accept(visitor&) const
   {
      assert(false && "Accept not implemented"); // NOLINT
   }

   void translation_unit_decl::accept(visitor& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visitor.visit(this);
   }

   void compound_class_decl::accept(visitor& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }
   }

   void class_decl::accept(visitor& visitor) const { visitor.visit(this); }
}; // namespace front::ast
