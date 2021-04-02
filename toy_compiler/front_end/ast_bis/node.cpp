#include <toy_compiler/front_end/ast_bis/node.hpp>

#include <toy_compiler/front_end/ast/node/integer_expr.hpp>
#include <toy_compiler/front_end/ast_bis/compound_stmt.hpp>
#include <toy_compiler/front_end/ast_bis/declaration.hpp>
#include <toy_compiler/front_end/ast_bis/factor.hpp>
#include <toy_compiler/front_end/ast_bis/function_decl.hpp>
#include <toy_compiler/front_end/ast_bis/literals.hpp>
#include <toy_compiler/front_end/ast_bis/operator.hpp>
#include <toy_compiler/front_end/ast_bis/statement.hpp>

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/take_while.hpp>

#include <memory>
#include <utility>

namespace front::ast
{
   namespace vi = ranges::views;

   node::node(const source_location& location) : m_location{location} {}
   node::node(std::string lexeme, const source_location& location) :
      m_lexeme{std::move(lexeme)},
      m_location{location}
   {}

   auto node::children() const -> const std::vector<node_ptr>& { return m_children; }
   auto node::lexeme() const -> std::string_view
   {

      if (!std::empty(m_lexeme))
      {
         return std::string_view{m_lexeme};
      }

      return {};
   }
   auto node::location() const -> const source_location& { return m_location; }

   void node::make_child(node_ptr child) { m_children.push_back(std::move(child)); }

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
         node_ptr main_decl = pop(recs);
         node_ptr compound_function_decl = pop(recs);
         node_ptr compound_class_decl = pop(recs);

         return std::make_unique<translation_unit_decl>(std::move(compound_class_decl),
                                                        std::move(compound_function_decl),
                                                        std::move(main_decl));
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

      if (action == sem::action::compound_function_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<function_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> place_holders = recs | vi::reverse |
               vi::take_while(detail::is_type<function_decl>) | vi::move | ranges::to_vector;

            std::vector<node_ptr> class_decls;
            for (auto& node : place_holders | vi::reverse)
            {
               pop(recs);
               class_decls.push_back(std::move(node));
            }

            return std::make_unique<compound_function_decl>(std::move(class_decls));
         }

         return std::make_unique<compound_function_decl>(std::vector<node_ptr>{});
      }

      if (action == sem::action::function_decl)
      {
         node_ptr statements = pop(recs);
         node_ptr function_head = pop(recs);

         return std::make_unique<function_decl>(std::move(function_head), std::move(statements));
      }

      if (action == sem::action::function_head_decl)
      {
         node_ptr return_type = pop(recs);
         node_ptr function_params = pop(recs);
         node_ptr class_method = pop(recs);
         node_ptr id = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<function_head_decl>(
            std::move(location), std::move(id), std::move(class_method), std::move(function_params),
            std::move(return_type));
      }

      if (action == sem::action::function_body_decl)
      {
         node_ptr statements = pop(recs);
         node_ptr variables = pop(recs);

         return std::make_unique<function_body_decl>(std::move(variables), std::move(statements));
      }

      if (action == sem::action::compound_class_decl)
      {
         std::vector<node_ptr> nodes;
         node_ptr null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<class_decl*>(recs.back().get()))
         {
            // clang-format off
            std::vector place_holder = recs 
               | vi::reverse 
               | vi::take_while(detail::is_type<class_decl>) 
               | vi::move
               | ranges::to_vector;
            // clang-format on

            std::vector<node_ptr> class_decls;
            for (auto& node : place_holder | vi::reverse)
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
            std::vector place_holders = recs | vi::reverse |
               vi::take_while(detail::is_type<member_decl>) | vi::move | ranges::to_vector;

            std::vector<node_ptr> nodes;
            for (auto& node : place_holders | vi::reverse)
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

      if (action == sem::action::member_function_decl)
      {
         node_ptr tail = pop(recs);
         node_ptr compound_param = pop(recs);
         node_ptr id = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<member_function_decl>(std::move(location), std::move(id),
                                                       std::move(compound_param), std::move(tail));
      }

      if (action == sem::action::compound_param_decl)
      {
         std::vector<node_ptr> nodes;
         node_ptr epsilon = pop(recs);

         assert(epsilon == nullptr); // NOLINT

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

      if (action == sem::action::compound_variable_decl)
      {
         std::vector<node_ptr> nodes;
         node_ptr null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<variable_decl*>(recs.back().get()))
         {
            std::vector place_holders = recs | vi::reverse |
               vi::take_while(detail::is_type<variable_decl>) | vi::move | ranges::to_vector;

            std::vector<node_ptr> nodes;
            for (auto& node : place_holders | vi::reverse)
            {
               pop(recs);
               nodes.push_back(std::move(node));
            }

            return std::make_unique<compound_variable_decl>(std::move(nodes));
         }

         return std::make_unique<compound_variable_decl>(std::vector<node_ptr>{});
      }

      if (action == sem::action::main_decl)
      {
         node_ptr func_body = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<main_decl>(std::move(id), std::move(func_body));
      }

      if (action == sem::action::rel_op)
      {
         node_ptr term_1 = pop(recs);
         node_ptr id = pop(recs);
         node_ptr term_0 = pop(recs);

         assert(dynamic_cast<id_decl*>(id.get())); // NOLINT

         return std::make_unique<mult_op>(std::move(term_0), std::move(id), std::move(term_1));
      }

      if (action == sem::action::add_op)
      {
         node_ptr factor_1 = pop(recs);
         node_ptr id = pop(recs);
         node_ptr factor_0 = pop(recs);

         assert(dynamic_cast<id_decl*>(id.get())); // NOLINT

         return std::make_unique<mult_op>(std::move(factor_0), std::move(id), std::move(factor_1));
      }

      if (action == sem::action::mult_op)
      {
         node_ptr factor_1 = pop(recs);
         node_ptr id = pop(recs);
         node_ptr factor_0 = pop(recs);

         assert(dynamic_cast<expr*>(factor_0.get())); // NOLINT
         assert(dynamic_cast<id_decl*>(id.get()));    // NOLINT
         assert(dynamic_cast<expr*>(factor_1.get())); // NOLINT

         return std::make_unique<mult_op>(std::move(factor_0), std::move(id), std::move(factor_1));
      }

      if (action == sem::action::int_expr)
      {
         return std::make_unique<integer_expr>(item.lexeme, item.pos);
      }

      if (action == sem::action::float_expr)
      {
         return std::make_unique<float_expr>(item.lexeme, item.pos);
      }

      if (action == sem::action::str_expr)
      {
         return std::make_unique<string_expr>(item.lexeme, item.pos);
      }

      if (action == sem::action::priority_expr)
      {
         node_ptr expr = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<priority_expr>(std::move(location), std::move(expr));
      }

      if (action == sem::action::not_expr)
      {
         node_ptr factor = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<not_expr>(std::move(id), std::move(factor));
      }

      if (action == sem::action::not_expr)
      {
         node_ptr factor = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<not_expr>(std::move(id), std::move(factor));
      }

      if (action == sem::action::sign_expr)
      {
         node_ptr factor = pop(recs);
         node_ptr id = pop(recs);

         assert(dynamic_cast<id_decl*>(id.get())); // NOLINT
         // NOLINTNEXTLINE
         assert(dynamic_cast<expr*>(factor.get()) || dynamic_cast<mult_op*>(factor.get()));

         return std::make_unique<sign_expr>(std::move(id), std::move(factor));
      }

      if (action == sem::action::func_or_var_expr)
      {
         std::vector place_holders = recs | vi::reverse | vi::take_while(detail::is_type<expr>) |
            vi::move | ranges::to_vector;

         std::vector<node_ptr> nodes;
         for (auto& node : place_holders | vi::reverse)
         {
            pop(recs);
            nodes.push_back(std::move(node));
         }

         return std::make_unique<func_or_var_expr>(std::move(nodes));
      }

      if (action == sem::action::compound_var_expr)
      {
         std::vector place_holders = recs | vi::reverse |
            vi::take_while(detail::is_type<variable_expr>) | vi::move | ranges::to_vector;

         std::vector<node_ptr> nodes;
         for (auto& node : place_holders | vi::reverse)
         {
            pop(recs);
            nodes.push_back(std::move(node));
         }

         return std::make_unique<compound_var_expr_decl>(std::move(nodes));
      }

      if (action == sem::action::func_expr)
      {
         node_ptr compound_param_expr = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<function_expr>(std::move(id), std::move(compound_param_expr));
      }

      if (action == sem::action::compound_parameter_expr_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1)
         {
            // clang-format off

            std::vector<node_ptr> placeholder = recs 
               | vi::reverse 
               | vi::take_while(detail::is_type<expr, op>) 
               | vi::move 
               | ranges::to_vector;

            // clang-format on

            std::vector<node_ptr> nodes;
            for (auto& node : placeholder | vi::reverse)
            {
               pop(recs);
               nodes.push_back(std::move(node));
            }

            return std::make_unique<compound_parameter_expr_decl>(std::move(nodes));
         }

         return std::make_unique<compound_parameter_expr_decl>(std::vector<node_ptr>{});
      }

      if (action == sem::action::var_expr)
      {
         node_ptr compound_array_index_access = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<variable_expr>(std::move(id),
                                                std::move(compound_array_index_access));
      }

      if (action == sem::action::compound_array_index_access_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<array_index_access_decl*>(recs.back().get()))
         {
            std::vector<node_ptr> placeholder;
            for (auto& node :
                 recs | vi::reverse | vi::take_while(detail::is_type<array_index_access_decl>))
            {
               placeholder.push_back(std::move(node));
            }

            std::vector<node_ptr> nodes;
            for (auto& node : placeholder | vi::reverse)
            {
               pop(recs);
               nodes.push_back(std::move(node));
            }

            return std::make_unique<compound_array_index_access_decl>(std::move(nodes));
         }

         return std::make_unique<compound_array_index_access_decl>(std::vector<node_ptr>{});
      }

      if (action == sem::action::array_index_access_decl)
      {
         node_ptr end = pop(recs);
         node_ptr expr = pop(recs);
         node_ptr beg = pop(recs);

         return std::make_unique<array_index_access_decl>(std::move(beg), std::move(expr),
                                                          std::move(end));
      }

      if (action == sem::action::ternary_expr)
      {
         node_ptr expr_1 = pop(recs);
         node_ptr expr_0 = pop(recs);
         node_ptr condition = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<ternary_expr>(std::move(location), std::move(condition),
                                               std::move(expr_0), std::move(expr_1));
      }

      if (action == sem::action::compound_stmt)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1 && dynamic_cast<stmt*>(recs.back().get()))
         {
            std::vector<node_ptr> placeholder;
            for (auto& node : recs | vi::reverse | vi::take_while(detail::is_type<stmt>))
            {
               placeholder.push_back(std::move(node));
            }

            std::vector<node_ptr> nodes;
            for (auto& node : placeholder | vi::reverse)
            {
               pop(recs);
               nodes.push_back(std::move(node));
            }

            return std::make_unique<compound_stmt>(std::move(nodes));
         }

         return std::make_unique<compound_stmt>(std::vector<node_ptr>{});
      }

      if (action == sem::action::if_stmt)
      {
         node_ptr else_block = pop(recs);
         node_ptr then_block = pop(recs);
         node_ptr expr = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<if_stmt>(std::move(location), std::move(expr),
                                          std::move(then_block), std::move(else_block));
      }

      if (action == sem::action::while_stmt)
      {
         node_ptr stmt_block = pop(recs);
         node_ptr expr = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<while_stmt>(std::move(location), std::move(expr),
                                             std::move(stmt_block));
      }

      if (action == sem::action::write_stmt)
      {
         node_ptr expr = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<write_stmt>(std::move(id), std::move(expr));
      }

      if (action == sem::action::read_stmt)
      {
         node_ptr compound_var = pop(recs);
         node_ptr loc = pop(recs);

         return std::make_unique<read_stmt>(std::move(loc), std::move(compound_var));
      }

      if (action == sem::action::return_stmt)
      {
         node_ptr expr = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<return_stmt>(std::move(id), std::move(expr));
      }

      if (action == sem::action::break_stmt)
      {
         return std::make_unique<break_stmt>(item.lexeme, item.pos);
      }

      if (action == sem::action::continue_stmt)
      {
         return std::make_unique<continue_stmt>(item.lexeme, item.pos);
      }

      if (action == sem::action::stmt_block_decl)
      {
         return std::make_unique<stmt_block_decl>(pop(recs));
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
} // namespace front::ast
