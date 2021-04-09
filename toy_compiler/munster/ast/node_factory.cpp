#include <toy_compiler/munster/ast/node_factory.hpp>

#include <toy_compiler/front_end/ast_bis/declaration.hpp>
#include <toy_compiler/munster/ast/decl/class_decl.hpp>
#include <toy_compiler/munster/ast/decl/compound_class_decl.hpp>
#include <toy_compiler/munster/ast/decl/compound_func_decl.hpp>
#include <toy_compiler/munster/ast/decl/compound_inheritance_decl.hpp>
#include <toy_compiler/munster/ast/decl/decl.hpp>
#include <toy_compiler/munster/ast/decl/dot_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_body_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_head_decl.hpp>
#include <toy_compiler/munster/ast/decl/id_decl.hpp>
#include <toy_compiler/munster/ast/decl/inheritance_decl.hpp>
#include <toy_compiler/munster/ast/decl/location_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_func_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_var_decl.hpp>
#include <toy_compiler/munster/ast/decl/translation_unit_decl.hpp>
#include <toy_compiler/munster/ast/decl/type_decl.hpp>
#include <toy_compiler/munster/ast/expr/compound_param_expr_decl.hpp>
#include <toy_compiler/munster/ast/expr/compound_var_expr_decl.hpp>
#include <toy_compiler/munster/ast/expr/float_expr.hpp>
#include <toy_compiler/munster/ast/expr/func_expr.hpp>
#include <toy_compiler/munster/ast/expr/integer_expr.hpp>
#include <toy_compiler/munster/ast/expr/not_expr.hpp>
#include <toy_compiler/munster/ast/expr/priority_expr.hpp>
#include <toy_compiler/munster/ast/expr/sign_expr.hpp>
#include <toy_compiler/munster/ast/expr/string_expr.hpp>
#include <toy_compiler/munster/ast/expr/ternary_expr.hpp>
#include <toy_compiler/munster/ast/expr/var_expr.hpp>
#include <toy_compiler/munster/ast/literal/int_literal.hpp>
#include <toy_compiler/munster/ast/literal/literal.hpp>
#include <toy_compiler/munster/ast/op/add_op.hpp>
#include <toy_compiler/munster/ast/op/assign_op.hpp>
#include <toy_compiler/munster/ast/op/dot_op.hpp>
#include <toy_compiler/munster/ast/op/mult_op.hpp>
#include <toy_compiler/munster/ast/op/op.hpp>
#include <toy_compiler/munster/ast/op/rel_op.hpp>
#include <toy_compiler/munster/ast/stmt/assign_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/break_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/compound_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/continue_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/func_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/if_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/read_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/return_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/while_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/write_stmt.hpp>

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>

#include <mpark/patterns.hpp>

#include <memory>
#include <utility>

namespace munster::ast
{
   namespace vi = ranges::views;

   auto pop(std::vector<node_ptr>& stack) -> std::unique_ptr<node>
   {
      auto temp = std::move(*(std::end(stack) - 1));
      stack.pop_back();
      return temp;
   }

   auto node_factory(front::sem::action action, const front::lex_item& item,
                     std::vector<node_ptr>& recs) -> node_ptr
   {
      using namespace mpark::patterns;

      if (action == front::sem::action::e_type_decl)
      {
         return std::make_unique<type_decl>(item.lexeme, item.pos);
      }

      if (action == front::sem::action::e_compound_func_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1)
         {
            std::vector<node_ptr> place_holders = recs | vi::reverse |
               vi::take_while(detail::is_type<func_decl>) | vi::move | ranges::to_vector;

            std::vector<func_decl::ptr> class_decls;
            for (auto& node : place_holders | vi::reverse)
            {
               pop(recs);
               class_decls.push_back(node_cast<func_decl>(std::move(node)));
            }

            return std::make_unique<compound_func_decl>(std::move(class_decls));
         }

         return std::make_unique<compound_func_decl>(std::vector<func_decl::ptr>{});
      }

      if (action == front::sem::action::e_func_decl)
      {
         node_ptr statements = pop(recs);
         node_ptr function_head = pop(recs);

         return std::make_unique<func_decl>(std::move(function_head), std::move(statements));
      }

      if (action == front::sem::action::e_func_head_decl)
      {
         node_ptr return_type = pop(recs);
         node_ptr function_params = pop(recs);
         node_ptr class_method = pop(recs);
         node_ptr id = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<func_head_decl>(
            std::move(location), std::move(id), std::move(class_method), std::move(function_params),
            std::move(return_type));
      }

      if (action == front::sem::action::e_func_body_decl)
      {
         node_ptr statements = pop(recs);
         node_ptr variables = pop(recs);

         return std::make_unique<func_body_decl>(std::move(variables), std::move(statements));
      }

      if (action == front::sem::action::e_compound_class_decl)
      {
         std::vector<node_ptr> nodes;
         node_ptr null = pop(recs);

         if (std::size(recs) >= 1)
         {
            // clang-format off
            std::vector place_holder = recs 
               | vi::reverse 
               | vi::take_while(detail::is_type<class_decl>) 
               | vi::move
               | ranges::to_vector;
            // clang-format on

            std::vector<class_decl::ptr> class_decls;
            for (auto& node : place_holder | vi::reverse)
            {
               pop(recs);
               class_decls.push_back(node_cast<class_decl>(std::move(node)));
            }

            return std::make_unique<compound_class_decl>(std::move(class_decls));
         }

         return std::make_unique<compound_class_decl>(std::vector<class_decl::ptr>{});
      }

      if (action == front::sem::action::e_class_decl)
      {
         node_ptr compound_member = pop(recs);
         node_ptr compound_inheritance = pop(recs);
         node_ptr class_name = pop(recs);
         node_ptr class_start = pop(recs);

         return std::make_unique<class_decl>(std::move(class_start), std::move(class_name),
                                             std::move(compound_inheritance),
                                             std::move(compound_member));
      }

      if (action == front::sem::action::e_compound_inheritance_decl)
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

      if (action == front::sem::action::e_inheritance_decl)
      {
         return std::make_unique<inheritance_decl>(item.lexeme, item.pos);
      }

      if (action == front::sem::action::e_compound_member_decl)
      {
         std::vector<node_ptr> nodes;
         auto null = pop(recs);

         if (std::size(recs) >= 1)
         {
            std::vector place_holders = recs | vi::reverse |
               vi::take_while(detail::is_type<member_func_decl, member_var_decl>) | vi::move |
               ranges::to_vector;

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

      if (action == front::sem::action::e_member_var_decl)
      {
         node_ptr compound_array = pop(recs);
         node_ptr id = pop(recs);
         node_ptr type = pop(recs);
         node_ptr visibility = pop(recs);

         return std::make_unique<member_var_decl>(std::move(visibility), std::move(type),
                                                  std::move(id), std::move(compound_array));
      }

      if (action == front::sem::action::e_visibily_decl)
      {
         node_ptr vis = std::make_unique<visibility_decl>(item.lexeme, item.pos);

         return vis;
      }

      if (action == front::sem::action::e_variable_decl)
      {
         node_ptr compound_array = pop(recs);
         node_ptr id = pop(recs);
         node_ptr type = pop(recs);

         return std::make_unique<variable_decl>(std::move(type), std::move(id),
                                                std::move(compound_array));
      }

      if (action == front::sem::action::e_compound_array_decl)
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

      if (action == front::sem::action::e_array_decl)
      {
         node_ptr end_loc = pop(recs);
         node_ptr size = pop(recs);
         node_ptr beg_loc = pop(recs);

         return std::make_unique<array_decl>(std::move(beg_loc), std::move(size),
                                             std::move(end_loc));
      }

      if (action == front::sem::action::e_member_func_decl)
      {
         node_ptr tail = pop(recs);
         node_ptr compound_param = pop(recs);
         node_ptr id = pop(recs);
         node_ptr location = pop(recs);
         node_ptr visibility = pop(recs);

         return std::make_unique<member_func_decl>(std::move(visibility), std::move(location),
                                                   std::move(id), std::move(compound_param),
                                                   std::move(tail));
      }

      if (action == front::sem::action::e_compound_param_decl)
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

      if (action == front::sem::action::e_compound_variable_decl)
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

      if (action == front::sem::action::e_main_decl)
      {
         node_ptr func_body = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<main_decl>(std::move(id), std::move(func_body));
      }

      if (action == front::sem::action::e_rel_op)
      {
         node_ptr term_1 = pop(recs);
         node_ptr id = pop(recs);
         node_ptr term_0 = pop(recs);

         assert(dynamic_cast<id_decl*>(id.get())); // NOLINT

         return std::make_unique<mult_op>(std::move(term_0), std::move(id), std::move(term_1));
      }

      if (action == front::sem::action::e_add_op)
      {
         node_ptr factor_1 = pop(recs);
         node_ptr id = pop(recs);
         node_ptr factor_0 = pop(recs);

         assert(dynamic_cast<id_decl*>(id.get())); // NOLINT

         return std::make_unique<add_op>(std::move(factor_0), std::move(id), std::move(factor_1));
      }

      if (action == front::sem::action::e_dot_op)
      {
         node_ptr expr_1 = pop(recs);

         if (dynamic_cast<dot_decl*>(recs.back().get()))
         {
            node_ptr dot = pop(recs);
            node_ptr expr_0 = pop(recs);

            return std::make_unique<dot_op>(std::move(expr_0), std::move(dot), std::move(expr_1));
         }

         return expr_1;
      }

      if (action == front::sem::action::e_mult_op)
      {
         node_ptr factor_1 = pop(recs);
         node_ptr id = pop(recs);
         node_ptr factor_0 = pop(recs);

         /*
         assert(dynamic_cast<expr*>(factor_0.get())); // NOLINT
         assert(dynamic_cast<id_decl*>(id.get()));    // NOLINT
         assert(dynamic_cast<expr*>(factor_1.get())); // NOLINT
         */

         return std::make_unique<mult_op>(std::move(factor_0), std::move(id), std::move(factor_1));
      }

      if (action == front::sem::action::e_assign_op)
      {
         node_ptr value_1 = pop(recs);
         node_ptr id = pop(recs);
         node_ptr value_0 = pop(recs);

         const bool val_0_check = detail::is_type<expr, op>(value_0);
         const bool val_1_check = detail::is_type<expr, op>(value_1);

         assert(val_0_check);                      // NOLINT
         assert(val_1_check);                      // NOLINT
         assert(dynamic_cast<id_decl*>(id.get())); // NOLINT

         return std::make_unique<assign_op>(std::move(value_0), std::move(id), std::move(value_1));
      }

      if (action == front::sem::action::e_int_expr)
      {
         return std::make_unique<integer_expr>(item.lexeme, item.pos);
      }

      if (action == front::sem::action::e_float_expr)
      {
         return std::make_unique<float_expr>(item.lexeme, item.pos);
      }

      if (action == front::sem::action::e_str_expr)
      {
         return std::make_unique<string_expr>(item.lexeme, item.pos);
      }

      if (action == front::sem::action::e_priority_expr)
      {
         node_ptr expr = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<priority_expr>(std::move(location), std::move(expr));
      }

      if (action == front::sem::action::e_not_expr)
      {
         node_ptr factor = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<not_expr>(std::move(id), std::move(factor));
      }

      if (action == front::sem::action::e_not_expr)
      {
         node_ptr factor = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<not_expr>(std::move(id), std::move(factor));
      }

      if (action == front::sem::action::e_sign_expr)
      {
         node_ptr factor = pop(recs);
         node_ptr id = pop(recs);

         assert(dynamic_cast<id_decl*>(id.get())); // NOLINT
         // NOLINTNEXTLINE
         assert(dynamic_cast<expr*>(factor.get()) || dynamic_cast<mult_op*>(factor.get()));

         return std::make_unique<sign_expr>(std::move(id), std::move(factor));
      }

      if (action == front::sem::action::e_func_or_assign_stmt)
      {
         std::vector place_holders = recs | vi::reverse |
            vi::take_while(detail::is_type<expr, op>) | vi::move | ranges::to_vector;

         std::vector<node_ptr> nodes;
         for (auto& node : place_holders | vi::reverse)
         {
            pop(recs);
            nodes.push_back(std::move(node));
         }

         return std::make_unique<func_stmt>(std::move(nodes));
      }

      if (action == front::sem::action::e_compound_var_expr)
      {
         std::vector place_holders = recs | vi::reverse |
            vi::take_while(detail::is_type<var_expr>) | vi::move | ranges::to_vector;

         std::vector<node_ptr> nodes;
         for (auto& node : place_holders | vi::reverse)
         {
            pop(recs);
            nodes.push_back(std::move(node));
         }

         return std::make_unique<compound_var_expr_decl>(std::move(nodes));
      }

      if (action == front::sem::action::e_func_expr)
      {
         node_ptr compound_param_expr = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<func_expr>(std::move(id), std::move(compound_param_expr));
      }

      if (action == front::sem::action::e_compound_parameter_expr_decl)
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

      if (action == front::sem::action::e_var_expr)
      {
         node_ptr compound_array_index_access = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<var_expr>(std::move(id), std::move(compound_array_index_access));
      }

      if (action == front::sem::action::e_compound_array_index_access_decl)
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

      if (action == front::sem::action::e_array_index_access_decl)
      {
         node_ptr end = pop(recs);
         node_ptr expr = pop(recs);
         node_ptr beg = pop(recs);

         return std::make_unique<array_index_access_decl>(std::move(beg), std::move(expr),
                                                          std::move(end));
      }

      if (action == front::sem::action::e_ternary_expr)
      {
         node_ptr expr_1 = pop(recs);
         node_ptr expr_0 = pop(recs);
         node_ptr condition = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<ternary_expr>(std::move(location), std::move(condition),
                                               std::move(expr_0), std::move(expr_1));
      }

      if (action == front::sem::action::e_compound_stmt)
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

      if (action == front::sem::action::e_if_stmt)
      {
         node_ptr else_block = pop(recs);
         node_ptr then_block = pop(recs);
         node_ptr expr = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<if_stmt>(std::move(location), std::move(expr),
                                          std::move(then_block), std::move(else_block));
      }

      if (action == front::sem::action::e_while_stmt)
      {
         node_ptr stmt_block = pop(recs);
         node_ptr expr = pop(recs);
         node_ptr location = pop(recs);

         return std::make_unique<while_stmt>(std::move(location), std::move(expr),
                                             std::move(stmt_block));
      }

      if (action == front::sem::action::e_write_stmt)
      {
         node_ptr expr = pop(recs);
         node_ptr id = pop(recs);

         return std::make_unique<write_stmt>(std::move(id), std::move(expr));
      }

      if (action == front::sem::action::e_read_stmt)
      {
         node_ptr compound_var = pop(recs);
         node_ptr loc = pop(recs);

         return std::make_unique<read_stmt>(std::move(loc), std::move(compound_var));
      }

      return match(action)(
         pattern(front::sem::action::e_translation_unit) = [&]() -> node_ptr {
            node_ptr main_decl = pop(recs);
            node_ptr compound_function_decl = pop(recs);
            auto compound_class = node_cast<compound_class_decl>(pop(recs));

            return std::make_unique<translation_unit_decl>(
               std::move(compound_class), std::move(compound_function_decl), std::move(main_decl));
         },
         pattern(front::sem::action::e_location_decl) = [&]() -> node_ptr {
            return std::make_unique<location_decl>(item.pos);
         },
         pattern(front::sem::action::e_id_decl) = [&]() -> node_ptr {
            return std::make_unique<id_decl>(item.lexeme, item.pos);
         },
         pattern(front::sem::action::e_dot_decl) = [&]() -> node_ptr {
            return std::make_unique<dot_decl>(item.lexeme, item.pos);
         },
         pattern(front::sem::action::e_assign_stmt) = [&]() -> node_ptr {
            auto assign = node_cast<assign_op>(pop(recs));

            return std::make_unique<assign_stmt>(std::move(assign));
         },
         pattern(front::sem::action::e_return_stmt) = [&]() -> node_ptr {
            node_ptr expr = pop(recs);
            node_ptr id = pop(recs);

            return std::make_unique<return_stmt>(std::move(id), std::move(expr));
         },
         pattern(front::sem::action::e_break_stmt) = [&]() -> node_ptr {
            return std::make_unique<break_stmt>(item.lexeme, item.pos);
         },
         pattern(front::sem::action::e_continue_stmt) = [&]() -> node_ptr {
            return std::make_unique<continue_stmt>(item.lexeme, item.pos);
         },
         pattern(front::sem::action::e_stmt_block_decl) = [&]() -> node_ptr {
            return std::make_unique<stmt_block_decl>(pop(recs));
         },
         pattern(front::sem::action::e_integer_literal) = [&]() -> node_ptr {
            return std::make_unique<integer_literal>(item.lexeme, item.pos);
         },
         pattern(_) = [&]() -> node_ptr {
            return nullptr;
         });
   }
} // namespace munster::ast
