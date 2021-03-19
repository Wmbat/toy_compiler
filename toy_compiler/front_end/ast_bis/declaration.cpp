#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <range/v3/view/tail.hpp>

#include <cassert>

namespace front::ast_bis
{
   namespace vi = ranges::views;

   decl::decl(const fr::source_location& location) : node{location} {}
   decl::decl(const std::string& lexeme, const fr::source_location& location) :
      node{lexeme, location}
   {}

   translation_unit_decl::translation_unit_decl(node_ptr compound_class)
   {
      // NOLINTNEXTLINE
      assert(dynamic_cast<compound_class_decl*>(compound_class.get()));

      make_child(std::move(compound_class));
   }

   auto translation_unit_decl::to_string() const -> std::string { return "translation_unit_decl"; }

   location_decl::location_decl(const fr::source_location& location) : decl{location} {}

   auto location_decl::to_string() const -> std::string { return fmt::format("{}", location()); }

   id_decl::id_decl(const std::string& lexeme, const fr::source_location& location) :
      decl{lexeme, location}
   {}

   auto id_decl::to_string() const -> std::string { return fmt::format("{}", lexeme()); }

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

   inheritance_decl::inheritance_decl(const std::string& name,
                                      const fr::source_location& location) :
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

   member_decl::member_decl(node_ptr visibility)
   {
      if (visibility != nullptr)
      {
         assert(dynamic_cast<visibility_decl*>(visibility.get())); // NOLINT

         make_child(std::move(visibility));
      }
   }

   auto member_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}>", "member_decl", child()->location().line,
                         child()->location().column);
   }

   visibility_decl::visibility_decl(const std::string& name, const fr::source_location& location) :
      decl{name, location}
   {}

   auto visibility_decl::to_string() const -> std::string
   {
      return fmt::format("{} <line:{}, col:{}> '{}'", "visibility_decl", location().line,
                         location().column, lexeme());
   }
}; // namespace front::ast_bis
