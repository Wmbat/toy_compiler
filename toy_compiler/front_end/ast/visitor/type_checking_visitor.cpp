#include <toy_compiler/front_end/ast/visitor/type_checking_visitor.hpp>

#include <toy_compiler/front_end/ast/decl/func_body_decl.hpp>
#include <toy_compiler/front_end/ast/decl/func_decl.hpp>
#include <toy_compiler/front_end/ast/decl/func_head_decl.hpp>
#include <toy_compiler/front_end/ast/decl/member_func_decl.hpp>
#include <toy_compiler/front_end/ast/decl/member_var_decl.hpp>
#include <toy_compiler/front_end/ast/expr/var_expr.hpp>
#include <toy_compiler/front_end/ast/expr/func_expr.hpp>
#include <toy_compiler/front_end/ast/op/assign_op.hpp>
#include <toy_compiler/front_end/ast/op/dot_op.hpp>
#include <toy_compiler/front_end/ast/op/mult_op.hpp>
#include <toy_compiler/front_end/ast/stmt/func_or_assign_stmt.hpp>
#include <toy_compiler/front_end/ast_bis/factor.hpp>

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>

namespace front::ast
{
   namespace vi = ranges::views;

   template <typename Any>
   auto to(const node_ptr& node) -> Any*
   {
      return dynamic_cast<Any*>(node.get());
   }

   type_checking_visitor::type_checking_visitor(symbol_table* p_root) : mp_root{p_root} {}

   void type_checking_visitor::visit(const translation_unit_decl* /*tud*/) {}
   void type_checking_visitor::visit(const compound_class_decl* /*ccd*/) {}
   void type_checking_visitor::visit(const class_decl* /*cd*/) {}
   void type_checking_visitor::visit(const compound_function_decl* /*cfd*/) {}
   void type_checking_visitor::visit(const func_decl* func_d)
   {
      const auto* head = to<func_head_decl>(func_d->children()[0]);
      const auto* body = to<func_body_decl>(func_d->children()[1]);

      std::string function_name;
      std::string class_name;
      if (head->class_name())
      {
         function_name = std::string{head->class_name().value()};
         class_name = std::string{head->lexeme()};

         const auto key = fmt::format("{} '{} ({})'", function_name, head->return_type(),
                                      fmt::join(head->params(), ", "));

         symbol_table* class_table = nullptr;
         if (auto class_res = mp_root->lookup(class_name))
         {
            class_table = class_res.val().link();
            if (class_table)
            {
               if (auto func_res = class_table->lookup(key))
               {
                  symbol_table* function_table = func_res.val().link();
                  std::vector<symbol_table*> tables = {class_table, function_table};
                  check_function_decl(body, tables);
               }
            }
         }
      }
      else
      {
         const auto key = fmt::format("{} '{} ({})'", head->lexeme(), head->return_type(),
                                      fmt::join(head->params(), ", "));

         if (auto func_res = mp_root->lookup(key))
         {
            std::vector<symbol_table*> tables{func_res.val().link()};
            check_function_decl(body, tables);
         }
      }
   }
   void type_checking_visitor::visit(const func_head_decl*) {}
   void type_checking_visitor::visit(const func_body_decl*) {}
   void type_checking_visitor::visit(const compound_variable_decl*) {}
   void type_checking_visitor::visit(const variable_decl*) {}
   void type_checking_visitor::visit(const compound_stmt*) {}
   void type_checking_visitor::visit(const main_decl*) {}
   void type_checking_visitor::visit(const func_or_assign_stmt*) {}
   void type_checking_visitor::visit(const return_stmt*) {}
   void type_checking_visitor::visit(const write_stmt*) {}

   void type_checking_visitor::check_function_decl(const func_body_decl* body,
                                                   std::span<symbol_table*> tables)
   {
      if (std::size(body->children()) == 1)
      {
      }
      else if (std::size(body->children()) == 2)
      {
         const auto& compound_statement = body->children()[1];

         for (const auto& stmt : compound_statement->children())
         {
            if (const auto func_or_assign = to<func_or_assign_stmt>(stmt))
            {
               const auto& func_or_assign_child = func_or_assign->children()[0];

               if (const auto assign = to<assign_op>(func_or_assign_child))
               {
                  handle_assign_op(assign, tables);
               }

               if (const auto func = to<function_expr>(func_or_assign_child))
               {
                  handle_function_expr(func);
               }
            }
         }
      }
   }

   void type_checking_visitor::handle_assign_op(const assign_op* assign,
                                                std::span<symbol_table*> tables)
   {
      const auto& left = assign->children()[0];
      [[maybe_unused]] const auto& right = assign->children()[1];

      std::string left_type;
      if (const auto* var_expr = to<variable_expr>(left)) // valid
      {
         handle_var_expr(var_expr, tables);
      }

      if (const auto* dot = to<dot_op>(left))
      {
         handle_dot_operator(dot, tables);
      }
   }

   auto type_checking_visitor::handle_var_expr(const variable_expr* var_expr,
                                               std::span<symbol_table*> tables) -> std::string_view
   {
      const std::string var_name{var_expr->lexeme()};

      // clang-format off
      const auto symbols = tables 
         | vi::filter([&](symbol_table* table) { return table->lookup(var_name); }) 
         | vi::transform([&](symbol_table* table) -> symbol* { 
               return &table->lookup(var_name).val();
           }) 
         | ranges::to<std::vector>;
      // clang-format on

      if (std::empty(symbols))
      {
         m_errors.push_back(
            {.type = parse_error_type::e_semantic_error,
             .pos = var_expr->location(),
             .lexeme = fmt::format("undeclared identifier '{}'", var_expr->lexeme())});

         return {};
      }
      else
      {
         return symbols.back()->type();
      }

      // TODO: Check for overshadow

      /*
      if (func_res || class_res)
      {
         std::string type;
         if (func_res)
         {
            type = func_res.val().type();
         }

         if (class_res)
         {
            // type = class_res.val().type();
         }

         if (auto res = mp_root->lookup(type); !res)
         {
            m_errors.push_back({.type = parse_error_type::e_semantic_error,
                                .pos = var_expr->location(),
                                .lexeme = fmt::format(
                                   "identifier '{}' is not a class and does not have members",
                                   var_expr->lexeme())});
         }
      }
      */
   }

   auto type_checking_visitor::handle_dot_operator(const dot_op* dot,
                                                   std::span<symbol_table*> tables)
      -> std::string_view
   {
      const auto& left = dot->children()[0];
      const auto& right = dot->children()[1];

      if (const auto* func_expr = to<function_expr>(left))
      {
      }

      if (const auto* var_expr = to<variable_expr>(left))
      {
         const auto return_type = handle_var_expr(var_expr, tables);

         if (const auto res = mp_root->lookup(std::string{return_type}))
         {
            if (res.val().kind() != symbol_type::e_class)
            {
               m_errors.push_back({.type = parse_error_type::e_semantic_error,
                                   .pos = var_expr->location(),
                                   .lexeme = fmt::format(
                                      "identifier '{}' to the left of `operator.` is not a class",
                                      var_expr->lexeme())});
            }
         }
         else
         {
            m_errors.push_back(
               {.type = parse_error_type::e_semantic_error,
                .pos = var_expr->location(),
                .lexeme = fmt::format("identifier '{}' to the left of `operator.` is not a class",
                                      var_expr->lexeme())});
         }
      }

      if (const auto* dot_operator = to<dot_op>(left))
      {
         return handle_dot_operator(dot_operator, tables);
      }

      if (const auto* func_expr = to<function_expr>(right))
      {
      }

      if (const auto* var_expr = to<variable_expr>(right))
      {
         return handle_var_expr(var_expr, tables);
      }

      return {};
   }

   auto type_checking_visitor::handle_function_expr(const function_expr* func) -> std::string
   {
      const auto name = func->lexeme();
      const auto symbol_values = mp_root->symbols() | vi::values;
      const auto symbol_it = ranges::find(symbol_values, name, &symbol::name);

      if (symbol_it != std::end(symbol_values))
      {
         const auto& symbol = *symbol_it;
         const auto decl_symbols = symbol.link()->symbols() | vi::values;
         const auto param_count = ranges::count_if(decl_symbols, [](const class symbol& s) {
            return s.kind() == symbol_type::e_parameter;
         });
         const auto it = ranges::find(decl_symbols, symbol_type::e_parameter, &symbol::kind);

         if (std::empty(func->children())) // NO PARAMS
         {
            if (it != std::end(decl_symbols))
            {
               m_errors.push_back(
                  {.type = parse_error_type::e_semantic_error,
                   .pos = func->location(),
                   .lexeme = fmt::format(
                      "invalid parameters for function '{}', expected {} parameters but got 0",
                      func->lexeme(), param_count)});
            }
         }
         else // WITH PARAMS
         {
            const auto& compound_params = func->children()[0];

            if (static_cast<std::size_t>(param_count) == std::size(compound_params->children()))
            {
               // check PARAMS
            }
            else
            {
               m_errors.push_back(
                  {.type = parse_error_type::e_semantic_error,
                   .pos = func->location(),
                   .lexeme = fmt::format(
                      "invalid parameters for function '{}', expected {} parameters but got {}",
                      func->lexeme(), func->location().line, param_count,
                      std::size(compound_params->children()))});
            }
         }

         return std::string{symbol.type().substr(0, symbol.type().find_first_of(' '))};
      }
      else
      {
         m_errors.push_back({.type = parse_error_type::e_semantic_error,
                             .pos = func->location(),
                             .lexeme = fmt::format("undeclared function '{}' called",
                                                   func->lexeme(), func->location().line)});

         return "";
      }
   }
   auto type_checking_visitor::handle_dot_operator(const dot_op* dot) -> std::string
   {
      const auto& left = dot->children()[0];
      [[maybe_unused]] const auto& right = dot->children()[1];

      // Path only taken for free functions
      if (const auto* func_expr = to<function_expr>(left))
      {
         const auto return_type = handle_function_expr(func_expr);

         if (const auto result = mp_root->lookup(return_type); !result)
         {
            // error
         }
      }

      if (const auto* var_expr = to<variable_expr>(left))
      {
         // const auto var_name = var_expr->lexeme();
      }

      if (const auto* dot_operator = to<dot_op>(left))
      {
         handle_dot_operator(dot_operator);
      }

      // dot_op has 2 children. Check if first child is not declared, error
      // if second child does not belong to class, error

      return "";
   }
} // namespace front::ast
