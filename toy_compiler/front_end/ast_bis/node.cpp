#include <toy_compiler/front_end/ast_bis/node.hpp>

#include <toy_compiler/front_end/ast_bis/declaration.hpp>
#include <toy_compiler/front_end/ast_bis/literals.hpp>

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <range/v3/view/reverse.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/take_while.hpp>

#include <utility>

namespace front::ast
{
   namespace vi = ranges::views;

   node::node(const source_location& location) : m_location{location} {}
   node::node(std::string lexeme, const source_location& location) :
      m_lexeme{std::move(lexeme)},
      m_location{location}
   {}

   auto node::child() const -> const node_ptr& { return m_child; }
   auto node::sibling() const -> const node_ptr& { return m_sibling; }
   auto node::lexeme() const -> std::string_view
   {
      if (!std::empty(m_lexeme))
      {
         return std::string_view{m_lexeme};
      }

      return {};
   }
   auto node::location() const -> const source_location& { return m_location; }

   void node::make_sibling(node_ptr sibling) { m_sibling = std::move(sibling); }
   void node::make_child(node_ptr child) { m_child = std::move(child); }

   auto pop(std::vector<node_ptr>& stack) -> std::unique_ptr<node>
   {
      auto temp = std::move(*(std::end(stack) - 1));
      stack.pop_back();
      return temp;
   }

   auto node_factory(front::sem::action action, const lex_item& item, std::vector<node_ptr>& recs)
      -> node_ptr
   {
      if (action == sem::action::translation_unit)
      {
         node_ptr compound_class_decl = pop(recs);

         return std::make_unique<translation_unit_decl>(std::move(compound_class_decl));
      };

      if (action == sem::action::location_decl)
      {
         return std::make_unique<location_decl>(item.pos);
      }

      if (action == sem::action::id_decl)
      {
         return std::make_unique<id_decl>(item.lexeme, item.pos);
      }

      if (action == sem::action::type_decl)
      {
         return std::make_unique<type_decl>(item.lexeme, item.pos);
      }

      if (action == sem::action::compound_class_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<class_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> place_holders;
            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<class_decl*>(node.get());
                              }))
            {
               place_holders.push_back(std::move(node));
            }

            std::vector<node_ptr> class_decls;
            for (auto& node : place_holders | vi::reverse)
            {
               pop(recs);
               class_decls.push_back(std::move(node));
            }

            return std::make_unique<compound_class_decl>(std::move(class_decls));
         }

         return std::make_unique<compound_class_decl>(std::vector<node_ptr>{});
      }

      if (action == sem::action::class_decl)
      {
         node_ptr compound_member = pop(recs);
         node_ptr compound_inheritance = pop(recs);
         node_ptr class_name = pop(recs);
         node_ptr class_start = pop(recs);

         return std::make_unique<class_decl>(std::move(class_start), std::move(class_name),
                                             std::move(compound_inheritance),
                                             std::move(compound_member));
      }

      if (action == sem::action::compound_inheritance_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<inheritance_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> placeholder;
            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<inheritance_decl*>(node.get());
                              }))
            {
               placeholder.push_back(std::move(node));
            }

            std::vector<node_ptr> nodes;
            for (auto& node : placeholder | vi::reverse)
            {
               pop(recs);
               nodes.push_back(std::move(node));
            }

            return std::make_unique<compound_inheritance_decl>(std::move(nodes));
         }

         return std::make_unique<compound_inheritance_decl>(std::vector<node_ptr>{});
      }

      if (action == sem::action::inheritance_decl)
      {
         return std::make_unique<inheritance_decl>(item.lexeme, item.pos);
      }

      if (action == sem::action::compound_member_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<member_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> placeholder;
            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<member_decl*>(node.get());
                              }))
            {
               placeholder.push_back(std::move(node));
            }

            std::vector<node_ptr> nodes;
            for (auto& node : placeholder | vi::reverse)
            {
               pop(recs);
               nodes.push_back(std::move(node));
            }

            return std::make_unique<compound_member_decl>(std::move(nodes));
         }

         return std::make_unique<compound_member_decl>(std::vector<node_ptr>{});
      }

      if (action == sem::action::member_decl)
      {
         node_ptr var_or_func = pop(recs);
         node_ptr visibility = pop(recs);

         return std::make_unique<member_decl>(std::move(visibility), std::move(var_or_func));
      }

      if (action == sem::action::visibily_decl)
      {
         node_ptr vis = std::make_unique<visibility_decl>(item.lexeme, item.pos);

         return vis;
      }

      if (action == sem::action::variable_decl)
      {
         node_ptr compound_array = pop(recs);
         node_ptr id = pop(recs);
         node_ptr type = pop(recs);

         return std::make_unique<variable_decl>(std::move(type), std::move(id),
                                                std::move(compound_array));
      }

      if (action == sem::action::compound_array_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<array_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> placeholder;
            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<array_decl*>(node.get());
                              }))
            {
               placeholder.push_back(std::move(node));
            }

            std::vector<node_ptr> nodes;
            for (auto& node : placeholder | vi::reverse)
            {
               pop(recs);
               nodes.push_back(std::move(node));
            }

            return std::make_unique<compound_array_decl>(std::move(nodes));
         }

         return std::make_unique<compound_array_decl>(std::vector<node_ptr>{});
      }

      if (action == sem::action::array_decl)
      {
         node_ptr end_loc = pop(recs);
         node_ptr size = pop(recs);
         node_ptr beg_loc = pop(recs);

         return std::make_unique<array_decl>(std::move(beg_loc), std::move(size),
                                             std::move(end_loc));
      }

      if (action == sem::action::function_decl)
      {
         node_ptr tail = pop(recs);
         node_ptr compound_param = pop(recs);
         node_ptr id = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<function_decl>(std::move(location), std::move(id),
                                                std::move(compound_param), std::move(tail));
      }

      if (action == sem::action::compound_param_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<variable_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> placeholder;
            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<variable_decl*>(node.get());
                              }))
            {
               placeholder.push_back(std::move(node));
            }

            std::vector<node_ptr> nodes;
            for (auto& node : placeholder | vi::reverse)
            {
               pop(recs);
               nodes.push_back(std::move(node));
            }

            return std::make_unique<compound_params_decl>(std::move(nodes));
         }

         return std::make_unique<compound_params_decl>(std::vector<node_ptr>{});
      }

      if (action == sem::action::integer_literal)
      {
         return std::make_unique<integer_literal>(item.lexeme, item.pos);
      }

      if (action == sem::action::epsilon)
      {
         return nullptr;
      }

      return nullptr;
   }
} // namespace front::ast_bis
