#include <toy_compiler/front_end/ast/node.hpp>

#include <toy_compiler/front_end/ast/array_size_node.hpp>
#include <toy_compiler/front_end/ast/declaration.hpp>
#include <toy_compiler/front_end/ast/epsilon_node.hpp>
#include <toy_compiler/front_end/ast/expr.hpp>
#include <toy_compiler/front_end/ast/func_body_node.hpp>
#include <toy_compiler/front_end/ast/list_node.hpp>
#include <toy_compiler/front_end/ast/method_body_var_node.hpp>
#include <toy_compiler/front_end/ast/operator.hpp>
#include <toy_compiler/front_end/ast/program_node.hpp>
#include <toy_compiler/front_end/ast/statement.hpp>
#include <toy_compiler/front_end/ast/type.hpp>
#include <toy_compiler/front_end/ast/value_node.hpp>
#include <toy_compiler/front_end/grammar/token_type.hpp>

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <range/v3/view/reverse.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/take_while.hpp>

#include <utility>

namespace fr::ast
{
   namespace vi = ranges::views;

   node::node(std::string lexeme, const source_location& location, std::uint32_t index) :
      m_lexeme{std::move(lexeme)},
      m_location{location},
      m_index{index}
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
   auto node::location() const -> const fr::source_location& { return m_location; }
   auto node::index() const -> std::uint32_t { return m_index; }

   void node::make_sibling(node_ptr sibling) { m_sibling = std::move(sibling); }
   void node::make_child(node_ptr child) { m_child = std::move(child); }

   auto pop(std::vector<node_ptr>& stack) -> std::unique_ptr<node>
   {
      auto temp = std::move(*(std::end(stack) - 1));
      stack.pop_back();
      return temp;
   }

   auto node_factory(sem::action_type type, const lex_item& item, std::vector<node_ptr>& recs)
      -> node_ptr
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
         auto first = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<class_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<class_decl*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<class_decl_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<class_decl_list>(std::move(nodes));
      }

      if (type == sem::action_type::inheritance_decl_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<inheritance_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<inheritance_decl*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<inheritance_declaration_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<inheritance_declaration_list>(std::move(nodes));
      }

      if (type == sem::action_type::class_body_decl_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<class_body_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<class_body_decl*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<class_body_declaration_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<class_body_declaration_list>(std::move(nodes));
      }

      if (type == sem::action_type::func_def_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<function*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            const auto is_node_type = [](const node_ptr& node) {
               return dynamic_cast<function*>(node.get());
            };

            for (auto& node : recs | vi::reverse | vi::take_while(is_node_type))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<function_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<function_list>(std::move(nodes));
      }

      if (type == sem::action_type::function_param_decl_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<function_param_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<function_param_decl*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<function_param_decl_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<function_param_decl_list>(std::move(nodes));
      }

      if (type == sem::action_type::arith_expr_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<arith_tail_expr*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            const auto is_node_type = [](const node_ptr& node) {
               return dynamic_cast<arith_tail_expr*>(node.get());
            };

            for (auto& node : recs | vi::reverse | vi::take_while(is_node_type))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<arith_expr_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<arith_expr_list>(std::move(nodes));
      }

      if (type == sem::action_type::arith_tail_expr)
      {
         node_ptr term = pop(recs);
         node_ptr add_op = pop(recs);

         return std::make_unique<arith_tail_expr>(std::move(add_op), std::move(term), item.pos);
      }

      if (type == sem::action_type::arith_expr)
      {
         node_ptr arith_tail_expr_list = pop(recs);
         node_ptr term = pop(recs);

         return std::make_unique<arith_tail_expr>(std::move(term), std::move(arith_tail_expr_list),
                                                  item.pos);
      }

      if (type == sem::action_type::term_tail_expr_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<term_tail_expr*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            const auto is_node_type = [](const node_ptr& node) {
               return dynamic_cast<term_tail_expr*>(node.get());
            };

            for (auto& node : recs | vi::reverse | vi::take_while(is_node_type))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<term_tail_expr_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<term_tail_expr_list>(std::move(nodes));
      }

      if (type == sem::action_type::term_tail_expr)
      {
         node_ptr factor = pop(recs);
         node_ptr mult_op = pop(recs);

         return std::make_unique<arith_tail_expr>(std::move(mult_op), std::move(factor), item.pos);
      }

      if (type == sem::action_type::term_expr)
      {
         node_ptr term_tail_expr_list = pop(recs);
         node_ptr factor = pop(recs);

         return std::make_unique<term_expr>(std::move(factor), std::move(term_tail_expr_list),
                                            item.pos);
      }

      if (type == sem::action_type::factor_expr)
      {
         if (item.type == grammar::token_type::float_lit)
         {
            node_ptr value = pop(recs);

            return std::make_unique<float_lit_factor_expr>(std::move(value), item.pos);
         }

         if (item.type == grammar::token_type::integer_lit)
         {
            node_ptr value = pop(recs);

            return std::make_unique<integer_lit_factor_expr>(std::move(value), item.pos);
         }

         if (item.type == grammar::token_type::str_lit)
         {
            node_ptr value = pop(recs);

            return std::make_unique<string_lit_factor_expr>(std::move(value), item.pos);
         }
      }

      if (type == sem::action_type::statement_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<statement*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<statement*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<statement_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<statement_list>(std::move(nodes));
      }

      if (type == sem::action_type::idnest_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<idnest_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<idnest_decl*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<idnest_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<idnest_list>(std::move(nodes));
      }

      if (type == sem::action_type::indice_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<indice_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<indice_decl*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<indice_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<indice_list>(std::move(nodes));
      }

      if (type == sem::action_type::class_decl)
      {
         node_ptr class_body_list = pop(recs);
         node_ptr inheritance_list = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<class_decl>(std::move(id), std::move(inheritance_list),
                                             std::move(class_body_list), item.pos);
      }

      if (type == sem::action_type::class_body_decl)
      {
         node_ptr member_decl = pop(recs);
         node_ptr visibility_decl = pop(recs);

         return std::make_unique<class_body_decl>(std::move(visibility_decl),
                                                  std::move(member_decl), item.pos);
      }

      if (type == sem::action_type::visibility_decl)
      {
         return std::make_unique<visibility_decl>(pop(recs), item.pos);
      }

      if (type == sem::action_type::member_decl)
      {
         return std::make_unique<member_decl>(pop(recs), item.pos);
      }

      if (type == sem::action_type::function_decl)
      {
         node_ptr id = pop(recs);
         node_ptr func_param_decl_list = pop(recs);
         node_ptr return_type_decl = pop(recs);

         return std::make_unique<function_decl>(std::move(id), std::move(func_param_decl_list),
                                                std::move(return_type_decl), item.pos);
      }

      if (type == sem::action_type::function_param_decl)
      {
         node_ptr type = pop(recs);
         node_ptr id = pop(recs);
         node_ptr array_size_stmt_list = pop(recs);

         return std::make_unique<function_param_decl>(std::move(type), std::move(id),
                                                      std::move(array_size_stmt_list), item.pos);
      }

      if (type == sem::action_type::return_type_decl)
      {
         return std::make_unique<return_type_decl>(pop(recs), item.pos);
      }

      if (type == sem::action_type::inherit_decl)
      {
         return std::make_unique<inheritance_decl>(pop(recs), item.pos);
      }

      if (type == sem::action_type::function)
      {
         node_ptr func_body = pop(recs);
         node_ptr func_head = pop(recs);

         return std::make_unique<function>(std::move(func_head), std::move(func_body), item.pos);
      }

      if (type == sem::action_type::func_head)
      {
         node_ptr return_type = pop(recs);
         node_ptr function_param = pop(recs);
         node_ptr class_method = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<func_head>(std::move(id), std::move(class_method),
                                            std::move(function_param), std::move(return_type),
                                            item.pos);
      }

      if (type == sem::action_type::func_body)
      {
         node_ptr statement_list = pop(recs);
         node_ptr method_body_var = pop(recs);

         return std::make_unique<func_body_node>(std::move(method_body_var),
                                                 std::move(statement_list));
      }

      if (type == sem::action_type::class_method)
      {
         return std::make_unique<class_method>(pop(recs), item.pos);
      }

      if (type == sem::action_type::statement)
      {
         if (item.type == grammar::token_type::id_continue)
         {
            return std::make_unique<continue_statement>(item.pos);
         }

         if (item.type == grammar::token_type::id_break)
         {
            return std::make_unique<break_statement>(item.pos);
         }

         if (item.type == grammar::token_type::id_read)
         {
            node_ptr expr = pop(recs);

            return std::make_unique<read_statement>(std::move(expr), item.pos);
         }
      }

      if (type == sem::action_type::write_statement)
      {
         node_ptr expr = pop(recs);

         return std::make_unique<write_statement>(std::move(expr), item.pos);
      }

      if (type == sem::action_type::return_statement)
      {
         node_ptr expr = pop(recs);

         return std::make_unique<return_statement>(std::move(expr), item.pos);
      }

      if (type == sem::action_type::if_statement)
      {
         node_ptr stat_block_else = pop(recs);
         node_ptr stat_block_then = pop(recs);
         node_ptr expr = pop(recs);

         return std::make_unique<if_statement>(std::move(expr), std::move(stat_block_then),
                                               std::move(stat_block_else), item.pos);
      }

      if (type == sem::action_type::while_statement)
      {
         node_ptr stat_block = pop(recs);
         node_ptr expr = pop(recs);

         return std::make_unique<while_statement>(std::move(expr), std::move(stat_block), item.pos);
      }

      if (type == sem::action_type::statement_block)
      {
         node_ptr expr = pop(recs);

         return std::make_unique<statement_block>(std::move(expr), item.pos);
      }

      if (type == sem::action_type::method_body_var)
      {
         auto first = pop(recs);

         return std::make_unique<method_body_var_node>(std::move(first));
      }

      if (type == sem::action_type::var_decl_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<variable_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

            for (auto& node : recs | vi::reverse | vi::take_while([](node_ptr& node) {
                                 return dynamic_cast<variable_decl*>(node.get());
                              }))
            {
               nodes.push_back(std::move(node));
            }

            for ([[maybe_unused]] auto& node : nodes | vi::tail)
            {
               pop(recs);
            }

            return std::make_unique<variable_declaration_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<variable_declaration_list>(std::move(nodes));
      }

      if (type == sem::action_type::var_decl)
      {
         node_ptr array_size_list = pop(recs);
         node_ptr value = pop(recs);
         node_ptr type = pop(recs);

         return std::make_unique<variable_decl>(std::move(type), std::move(value),
                                                std::move(array_size_list), item.pos);
      }

      if (type == sem::action_type::var_expr)
      {
         node_ptr idnest_list = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<variable_expr>(std::move(id), std::move(idnest_list), item.pos);
      }

      if (type == sem::action_type::expr)
      {
         node_ptr first = pop(recs);

         if (dynamic_cast<epsilon_node*>(first.get()))
         {
            node_ptr term = pop(recs);

            return std::make_unique<expr>(std::move(term), item.pos);
         }
         else
         {
            node_ptr arith_expr_1 = pop(recs);
            node_ptr rel_op = pop(recs);
            node_ptr arith_expr_0 = pop(recs);

            return std::make_unique<expr>(std::move(arith_expr_0), std::move(rel_op),
                                          std::move(arith_expr_1), item.pos);
         }
      }

      if (type == sem::action_type::array_size)
      {
         return std::make_unique<array_size_node>(pop(recs));
      }

      if (type == sem::action_type::idnest_decl)
      {
         node_ptr indice_list = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<idnest_decl>(std::move(indice_list), std::move(id), item.pos);
      }

      if (type == sem::action_type::array_size_stmt_list)
      {
         std::vector<node_ptr> nodes;
         auto first = pop(recs);

         if (dynamic_cast<array_size_node*>(recs.back().get()))
         {
            std::vector<node_ptr> nodes;
            nodes.push_back(pop(recs));

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

            return std::make_unique<array_size_statement_list>(std::move(nodes));
         }

         nodes.push_back(std::move(first));

         return std::make_unique<array_size_statement_list>(std::move(nodes));
      }

      if (type == sem::action_type::type)
      {
         if (item.type == grammar::token_type::id_void)
         {
            return std::make_unique<void_type>(item.pos);
         }
         if (item.type == grammar::token_type::id_integer)
         {
            return std::make_unique<integer_type>(item.pos);
         }
         if (item.type == grammar::token_type::id_float)
         {
            return std::make_unique<float_type>(item.pos);
         }
         if (item.type == grammar::token_type::id_string)
         {
            return std::make_unique<string_type>(item.pos);
         }

         return std::make_unique<user_defined_type>(item.lexeme, item.pos);
      }

      if (type == sem::action_type::value)
      {
         if (item.type == grammar::token_type::integer_lit)
         {
            return std::make_unique<integer_lit>(item.lexeme, item.pos);
         }

         return std::make_unique<value_node>(item.lexeme, item.pos);
      }

      if (type == sem::action_type::add_op)
      {
         return std::make_unique<add_op>(pop(recs), item.pos);
      }

      if (type == sem::action_type::mult_op)
      {
         return std::make_unique<mult_op>(pop(recs), item.pos);
      }

      if (type == sem::action_type::rel_op)
      {
         return std::make_unique<rel_op>(pop(recs), item.pos);
      }

      if (type == sem::action_type::epsilon)
      {
         return std::make_unique<epsilon_node>();
      }

      return nullptr;
   }
} // namespace fr::ast
