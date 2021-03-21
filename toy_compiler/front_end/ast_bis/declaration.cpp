#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <range/v3/view/tail.hpp>
#include <range/v3/view/take.hpp>

#include <cassert>

namespace front::ast
{
   namespace vi = ranges::views;

   decl::decl(const source_location& location) : node{location} {}
   decl::decl(const std::string& lexeme, const source_location& location) : node{lexeme, location}
   {}

   translation_unit_decl::translation_unit_decl(node_ptr compound_class)
   {
      // NOLINTNEXTLINE
      assert(dynamic_cast<compound_class_decl*>(compound_class.get()));

      make_child(std::move(compound_class));
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
      if (std::size(class_decls) > 0)
      {
         node* it = class_decls.front().get();
         for (node_ptr& node : class_decls | vi::tail)
         {
            // NOLINTNEXTLINE
            assert(dynamic_cast<class_decl*>(it));

            auto temp = node.get();
            it->make_sibling(std::move(node));
            it = temp;
         }

         make_child(std::move(class_decls.front()));
      }
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

      if (compound_member->child())
      {
         if (compound_inheritance->child())
         {
            compound_inheritance->make_sibling(std::move(compound_member));
            make_child(std::move(compound_inheritance));
         }
         else
         {
            make_child(std::move(compound_member));
         }
      }
   }

   auto class_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "class_decl", location().line,
                         location().column, lexeme());
   }

   compound_inheritance_decl::compound_inheritance_decl(std::vector<node_ptr>&& class_decls)
   {
      if (std::size(class_decls) > 0)
      {
         node* it = class_decls.front().get();
         for (node_ptr& node : class_decls | vi::tail)
         {
            // NOLINTNEXTLINE
            assert(dynamic_cast<inheritance_decl*>(it));

            auto temp = node.get();
            it->make_sibling(std::move(node));
            it = temp;
         }

         make_child(std::move(class_decls.front()));
      }
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
      if (std::size(member_decls) > 0)
      {
         node* it = member_decls.front().get();
         for (node_ptr& node : member_decls | vi::tail)
         {
            // NOLINTNEXTLINE
            assert(dynamic_cast<member_decl*>(it));

            auto temp = node.get();
            it->make_sibling(std::move(node));
            it = temp;
         }

         make_child(std::move(member_decls.front()));
      }
   }

   auto compound_member_decl::to_string() const -> std::string { return "compound_member_decl"; }

   member_decl::member_decl(node_ptr visibility, node_ptr var_or_func)
   {
      if (visibility != nullptr)
      {
         assert(dynamic_cast<visibility_decl*>(visibility.get())); // NOLINT

         visibility->make_sibling(std::move(var_or_func));
         make_child(std::move(visibility));
      }
      else
      {
         make_child(std::move(var_or_func));
      }
   }

   auto member_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}>", "member_decl", child()->location().line,
                         child()->location().column);
   }

   visibility_decl::visibility_decl(const std::string& name, const source_location& location) :
      decl{name, location}
   {}

   auto visibility_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "visibility_decl", location().line,
                         location().column, lexeme());
   }

   variable_decl::variable_decl(node_ptr type, node_ptr id, node_ptr compound_array_decl) :
      decl{type->location()},
      m_type(type->lexeme()),
      m_id{id->lexeme()}
   {
      if (compound_array_decl->child())
      {
         make_child(std::move(compound_array_decl));
      }
   }

   auto variable_decl::type() const -> std::string { return m_type; }
   auto variable_decl::to_string() const -> std::string
   {
      return fmt::format("variable_decl <line:{}, col:{}> {} '{}'", location().line,
                         location().line, m_id, m_type);
   }

   compound_array_decl::compound_array_decl(std::vector<node_ptr>&& array_decls)
   {
      if (std::size(array_decls) > 0)
      {
         node* it = array_decls.front().get();
         for (node_ptr& node : array_decls | vi::tail)
         {
            // NOLINTNEXTLINE
            assert(dynamic_cast<array_decl*>(it));

            auto temp = node.get();
            it->make_sibling(std::move(node));
            it = temp;
         }

         make_child(std::move(array_decls.front()));
      }
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

   function_decl::function_decl(node_ptr location, node_ptr id, node_ptr compound_param,
                                node_ptr tail) :
      decl{std::string{id->lexeme()}, location->location()},
      m_return_type{tail->lexeme()}
   {
      if (compound_param->child())
      {
         node* it = compound_param->child().get();
         while (it)
         {
            m_params.emplace_back(dynamic_cast<variable_decl*>(it)->type());
            it = it->sibling().get();
         }

         make_child(std::move(compound_param));
      }
   }

   auto function_decl::to_string() const -> std::string
   {
      std::string params;
      for (const auto& param : m_params)
      {
         params += param;
         params += ", ";
      }

      return fmt::format("func_decl <line:{}, col:{}> {} '{} ({})'", location().line,
                         location().column, lexeme(), m_return_type,
                         params.substr(0, std::size(params) - 2));
   }

   compound_params_decl::compound_params_decl(std::vector<node_ptr>&& param_decls)
   {
      if (std::size(param_decls) > 0)
      {
         node* it = param_decls.front().get();
         for (node_ptr& node : param_decls | vi::tail)
         {
            // NOLINTNEXTLINE
            assert(dynamic_cast<variable_decl*>(it));

            auto temp = node.get();
            it->make_sibling(std::move(node));
            it = temp;
         }

         make_child(std::move(param_decls.front()));
      }
   }

   auto compound_params_decl::to_string() const -> std::string { return "compound_array_decl"; }
}; // namespace front::ast_bis
