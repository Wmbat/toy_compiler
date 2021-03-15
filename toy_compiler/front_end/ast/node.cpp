#include <toy_compiler/front_end/ast/node.hpp>

#include <toy_compiler/front_end/ast/array_size_node.hpp>
#include <toy_compiler/front_end/ast/epsilon_node.hpp>
#include <toy_compiler/front_end/ast/func_body_node.hpp>
#include <toy_compiler/front_end/ast/list_node.hpp>
#include <toy_compiler/front_end/ast/method_body_var_node.hpp>
#include <toy_compiler/front_end/ast/program_node.hpp>
#include <toy_compiler/front_end/ast/value_node.hpp>
#include <toy_compiler/front_end/ast/var_decl_node.hpp>

#include <fmt/core.h>

#include <range/v3/view/reverse.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/take_while.hpp>

#include <utility>

namespace fr::ast
{
   namespace vi = ranges::views;

   node::node(fr::lex_item item) : m_item{std::move(item)} {}

   void node::make_sibling(node_ptr sibling) { m_sibling = std::move(sibling); }
   void node::make_child(node_ptr child) { m_child = std::move(child); }

   auto node::child() const -> const node_ptr& { return m_child; }
   auto node::sibling() const -> const node_ptr& { return m_sibling; }
   auto node::token() const -> const fr::lex_item& { return m_item; }

   auto pop(std::vector<node_ptr>& stack) -> std::unique_ptr<node>
   {
      auto temp = std::move(*(std::end(stack) - 1));
      stack.pop_back();
      return temp;
   }

   auto node_factory(sem::action_type type, lex_item item, std::vector<node_ptr>& recs) -> node_ptr
   {
      if (type == sem::action_type::program)
      {
         node_ptr func_body = pop(recs);
         node_ptr func_def_list = pop(recs);
         node_ptr class_decl_list = pop(recs);

         return std::make_unique<program_node>(std::move(class_decl_list), std::move(func_def_list),
                                               std::move(func_body));
      }

      if (type == sem::action_type::class_decl_list)
      {
         std::vector<node_ptr> nodes;
         nodes.push_back(pop(recs));

         return std::make_unique<list_node>(list_node_type::class_decl, std::move(nodes));
      }

      if (type == sem::action_type::func_def_list)
      {
         std::vector<node_ptr> nodes;
         nodes.push_back(pop(recs));

         return std::make_unique<list_node>(list_node_type::func_def, std::move(nodes));
      }

      if (type == sem::action_type::func_body)
      {
         node_ptr statement_list = pop(recs);
         node_ptr method_body_var = pop(recs);

         return std::make_unique<func_body_node>(std::move(method_body_var),
                                                 std::move(statement_list));
      }

      if (type == sem::action_type::statement_list)
      {
         if (auto first = pop(recs); dynamic_cast<epsilon_node*>(first.get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(std::move(first));

            return std::make_unique<list_node>(list_node_type::statement, std::move(nodes));
         }
         else
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(std::move(first));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<var_decl_node*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<list_node>(list_node_type::statement, std::move(nodes));
         }
      }

      if (type == sem::action_type::method_body_var)
      {
         return std::make_unique<method_body_var_node>(pop(recs));
      }

      if (type == sem::action_type::var_decl_list)
      {
         if (auto first = pop(recs); dynamic_cast<epsilon_node*>(first.get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(std::move(first));

            return std::make_unique<list_node>(list_node_type::var_decl, std::move(nodes));
         }
         else
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(std::move(first));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<var_decl_node*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<list_node>(list_node_type::var_decl, std::move(nodes));
         }
      }

      if (type == sem::action_type::var_decl)
      {
         node_ptr array_size_list = pop(recs);
         node_ptr value = std::make_unique<value_node>(std::move(item));
         node_ptr type = pop(recs);

         return std::make_unique<var_decl_node>(std::move(type), std::move(value),
                                                std::move(array_size_list));
      }

      if (type == sem::action_type::array_size)
      {
         return std::make_unique<array_size_node>(item);
      }

      if (type == sem::action_type::array_size_list)
      {
         if (auto first = pop(recs); dynamic_cast<epsilon_node*>(first.get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(std::move(first));

            return std::make_unique<list_node>(list_node_type::array_size, std::move(nodes));
         }
         else
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(std::move(first));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<array_size_node*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<list_node>(list_node_type::array_size, std::move(nodes));
         }
      }

      if (type == sem::action_type::epsilon)
      {
         return std::make_unique<epsilon_node>();
      }

      return nullptr;
   }
} // namespace fr::ast
