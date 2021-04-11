#include <toy_compiler/munster/visitor/semantic_checking/type_checking_visitor.hpp>

#include <toy_compiler/munster/ast/decl/func_body_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_head_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_func_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_var_decl.hpp>
#include <toy_compiler/munster/ast/expr/float_expr.hpp>
#include <toy_compiler/munster/ast/expr/func_expr.hpp>
#include <toy_compiler/munster/ast/expr/integer_expr.hpp>
#include <toy_compiler/munster/ast/expr/string_expr.hpp>
#include <toy_compiler/munster/ast/expr/var_expr.hpp>
#include <toy_compiler/munster/ast/op/assign_op.hpp>
#include <toy_compiler/munster/ast/op/dot_op.hpp>
#include <toy_compiler/munster/ast/op/mult_op.hpp>
#include <toy_compiler/munster/ast/stmt/assign_stmt.hpp>
#include <toy_compiler/munster/ast/stmt/func_stmt.hpp>

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <range/v3/action/split.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/transform.hpp>

#include <fmt/core.h>

#include <mpark/patterns.hpp>

namespace ra = ranges::actions;
namespace vi = ranges::views;
using namespace mpark::patterns;

namespace munster
{
   template <typename Any>
   auto to(const ast::node_ptr& node) -> Any*
   {
      return dynamic_cast<Any*>(node.get());
   }

   type_checking_visitor::type_checking_visitor(symbol_table* p_root) : mp_root{p_root} {}

   void type_checking_visitor::visit(const ast::translation_unit_decl& /*tud*/) {}
   void type_checking_visitor::visit(const ast::compound_class_decl& /*ccd*/) {}
   void type_checking_visitor::visit(const ast::class_decl& /*cd*/) {}
   void type_checking_visitor::visit(const ast::compound_inheritance_decl&) {}
   void type_checking_visitor::visit(const ast::inheritance_decl&) {}
   void type_checking_visitor::visit(const ast::compound_member_decl&) {}
   void type_checking_visitor::visit(const ast::member_func_decl&) {}
   void type_checking_visitor::visit(const ast::member_var_decl&) {}
   void type_checking_visitor::visit(const ast::compound_function_decl& /*cfd*/) {}
   void type_checking_visitor::visit(const ast::func_decl& func_d)
   {
      const auto* head = to<ast::func_head_decl>(func_d.children()[0]);
      const auto* body = to<ast::func_body_decl>(func_d.children()[1]);

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
   void type_checking_visitor::visit(const ast::func_head_decl& head)
   {
      if (head.class_name())
      {
         const auto class_name = std::string{head.lexeme()};
         const auto func_name = head.class_name().value();

         if (auto class_res = mp_root->lookup(class_name))
         {
            if (auto* class_table = class_res.val().link())
            {
               m_tables.push_back(class_table);

               const auto func_key = fmt::format("{} '{} ({})'", func_name, head.return_type(),
                                                 fmt::join(head.params(), ", "));

               if (auto func_res = class_table->lookup(func_key))
               {
                  m_tables.push_back(func_res.val().link());
               }
               else
               {
                  m_errors.push_back(
                     {.type = front::parse_error_type::e_semantic_error,
                      .pos = head.location(),
                      .lexeme = fmt::format("member function '{}' has no implementation '{}'",
                                            func_name, class_name)});
               }
            }
         }
         else
         {
            m_errors.push_back({.type = front::parse_error_type::e_semantic_error,
                                .pos = head.location(),
                                .lexeme = fmt::format("undeclared class '{}'", class_name)});
         }
      }
      else
      {
         const auto func_key = fmt::format("{} '{} ({})'", head.lexeme(), head.return_type(),
                                           fmt::join(head.params(), ", "));

         if (auto func_res = mp_root->lookup(func_key))
         {
            m_tables.push_back(func_res.val().link());
         }
      }
   }
   void type_checking_visitor::visit(const ast::func_body_decl& /*body*/)
   {
      if (!std::empty(m_tables))
      {
         m_tables.pop_back();
      }

      if (!std::empty(m_tables))
      {
         m_tables.pop_back();
      }
   }
   void type_checking_visitor::visit(const ast::compound_variable_decl&) {}
   void type_checking_visitor::visit(const ast::variable_decl&) {}
   void type_checking_visitor::visit(const ast::main_decl&) {}

   void type_checking_visitor::visit(const ast::func_expr& /*node*/) {}
   void type_checking_visitor::visit(const ast::var_expr& node)
   {
      const std::string var_name{node.lexeme()};

      fmt::print("var_expr: {}\n", var_name);

      if (std::size(m_tables) == 1u)
      {
         symbol_table* func_table = *(std::end(m_tables) - 1);

         if (const auto res = func_table->lookup(var_name))
         {
            m_symbols.push_back(&res.val());
         }
         else
         {
            m_symbols.push_back({});
            m_errors.push_back(
               front::parse_error{.type = front::parse_error_type::e_semantic_error,
                                  .pos = node.location(),
                                  .lexeme = fmt::format("undeclared variable '{}'", var_name)});
         }
      }
      else if (std::size(m_tables) == 2u)
      {
         symbol_table* p_func_table = *(std::end(m_tables) - 1);
         symbol_table* p_class_table = *(std::end(m_tables) - 2);

         symbol* p_symbol = nullptr;
         if (const auto res = p_func_table->lookup(var_name))
         {
            p_symbol = &res.val();
         }

         if (!p_symbol)
         {
            if (const auto res = p_class_table->lookup(var_name))
            {
               p_symbol = &res.val();
            }
         }

         if (!p_symbol)
         {
            m_errors.push_back(
               front::parse_error{.type = front::parse_error_type::e_semantic_error,
                                  .pos = node.location(),
                                  .lexeme = fmt::format("undeclared variable '{}'", var_name)});
         }

         m_symbols.push_back(p_symbol);
      }
   }
   void type_checking_visitor::visit(const ast::float_expr& node)
   {
      auto* func_table = m_tables.back();

      const auto name = fmt::format("temp{}", m_temporary_counter++);
      const auto res =
         func_table->insert(name, symbol{name, symbol_type::e_temporary, node.location(), "float"});

      m_symbols.push_back(&res.val());
   }
   void type_checking_visitor::visit(const ast::integer_expr& node)
   {
      auto* func_table = m_tables.back();

      const auto name = fmt::format("temp{}", m_temporary_counter++);
      const auto res = func_table->insert(
         name, symbol{name, symbol_type::e_temporary, node.location(), "integer"});

      m_symbols.push_back(&res.val());
   }
   void type_checking_visitor::visit(const ast::string_expr& node)
   {
      auto* func_table = m_tables.back();

      const auto name = fmt::format("temp{}", m_temporary_counter++);
      const auto res = func_table->insert(
         name, symbol{name, symbol_type::e_temporary, node.location(), "string"});

      m_symbols.push_back(&res.val());
   }

   void type_checking_visitor::visit(const ast::func_stmt& /*node*/) { fmt::print("func_stmt"); }
   void type_checking_visitor::visit(const ast::compound_stmt& /*node*/) {}
   void type_checking_visitor::visit(const ast::assign_stmt& node)
   {
      // TODO: remove
      if (std::size(m_symbols) != 2)
      {
         if (!std::empty(m_symbols))
         {
            m_symbols.pop_back();
         }

         return;
      }

      const auto get_var_type = [](symbol* s) {
         if (s->kind() == symbol_type::e_member_variable)
         {
            const auto split = ranges::actions::split(s->type(), ' ');
            const auto vis = *(std::end(split) - 2) | ranges::to<std::string>();
            return *(std::end(split) - 1) | ranges::to<std::string>();
         }
         else
         {
            return std::string{s->type()};
         }
      };

      auto* p_left = *(std::end(m_symbols) - 2);
      auto* p_right = *(std::end(m_symbols) - 1);

      if (p_left && p_right)
      {
         const std::string left_type = get_var_type(p_left);
         const std::string right_type = get_var_type(p_right);

         if (left_type != right_type)
         {
            m_errors.push_back(front::parse_error{
               .type = front::parse_error_type::e_semantic_error,
               .pos = node.children()[0]->location(),
               .lexeme = fmt::format("variable '{}' of type '{}' cannot be assigned to "
                                     "variable '{}' of type '{}'",
                                     p_right->name(), right_type, p_left->name(), left_type)});
         }
      }

      m_symbols.pop_back();
      m_symbols.pop_back();

      fmt::print("assign_stmt\n");
   }
   void type_checking_visitor::visit(const ast::if_stmt& /*node*/) {}
   void type_checking_visitor::visit(const ast::while_stmt& /*node*/) {}
   void type_checking_visitor::visit(const ast::read_stmt& /*node*/) {}
   void type_checking_visitor::visit(const ast::write_stmt& /*node*/) {}
   void type_checking_visitor::visit(const ast::return_stmt& /*node*/) {}
   void type_checking_visitor::visit(const ast::break_stmt& /*node*/) {}
   void type_checking_visitor::visit(const ast::continue_stmt& /*node*/) {}

   void type_checking_visitor::visit(const ast::dot_op& node)
   {
      auto* p_left_symbol = *(std::end(m_symbols) - 2);
      auto* p_right_symbol = *(std::end(m_symbols) - 1);

      if (p_left_symbol && p_right_symbol)
      {
         if (p_left_symbol->kind() == symbol_type::e_member_variable)
         {
            const auto split = ranges::actions::split(p_left_symbol->type(), ' ');
            const auto vis = *(std::end(split) - 2) | ranges::to<std::string>();
            const auto type = *(std::end(split) - 1) | ranges::to<std::string>();

            if (const auto res = mp_root->lookup(type); !res)
            {
               m_errors.push_back(front::parse_error{
                  .type = front::parse_error_type::e_semantic_error,
                  .pos = node.children()[0]->location(),
                  .lexeme = fmt::format("'{}' is not a class and does not have member variables",
                                        p_left_symbol->name())});
            }
         }
         if (p_left_symbol->kind() == symbol_type::e_variable)
         {
            const std::string type{p_left_symbol->type()};
            if (const auto res = mp_root->lookup(type); !res)
            {
               m_errors.push_back(front::parse_error{
                  .type = front::parse_error_type::e_semantic_error,
                  .pos = node.children()[0]->location(),
                  .lexeme = fmt::format("'{}' is not a class", p_left_symbol->name())});
            }
         }

         m_symbols.pop_back();
         m_symbols.pop_back();

         m_symbols.push_back(p_right_symbol);
      }
      else
      {
         m_symbols.pop_back();
         m_symbols.pop_back();

         m_symbols.push_back(nullptr);
      }
   }
   void type_checking_visitor::visit(const ast::assign_op& /*node*/) {}

   ////////////////////////////////////////////////////

   void type_checking_visitor::check_function_decl(const ast::func_body_decl* body,
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
            if (const auto func_or_assign = to<ast::func_stmt>(stmt))
            {
               const auto& func_or_assign_child = func_or_assign->children()[0];

               if (const auto assign = to<ast::assign_op>(func_or_assign_child))
               {
                  handle_assign_op(assign, tables);
               }

               if (const auto func = to<ast::func_expr>(func_or_assign_child))
               {
                  handle_function_expr(func);
               }
            }
         }
      }
   }

   void type_checking_visitor::handle_assign_op(const ast::assign_op* assign,
                                                std::span<symbol_table*> tables)
   {
      const auto& left = assign->children()[0];
      [[maybe_unused]] const auto& right = assign->children()[1];

      std::string left_type;
      if (const auto* var = to<ast::var_expr>(left)) // valid
      {
         handle_var_expr(var, tables);
      }

      if (const auto* dot = to<ast::dot_op>(left))
      {
         handle_dot_operator(dot, tables);
      }
   }

   auto type_checking_visitor::handle_var_expr(const ast::var_expr* var_expr,
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
            {.type = front::parse_error_type::e_semantic_error,
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
            m_errors.push_back(
               {.type = parse_error_type::e_semantic_error,
                .pos = var_expr->location(),
                .lexeme = fmt::format("identifier '{}' is not a class and does not have members",
                                      var_expr->lexeme())});
         }
      }
      */
   }

   auto type_checking_visitor::handle_dot_operator(const ast::dot_op* dot,
                                                   std::span<symbol_table*> tables)
      -> std::string_view
   {
      const auto& left = dot->children()[0];
      const auto& right = dot->children()[1];

      if (const auto* func = to<ast::func_expr>(left))
      {
      }

      if (const auto* var = to<ast::var_expr>(left))
      {
         const auto return_type = handle_var_expr(var, tables);

         if (const auto res = mp_root->lookup(std::string{return_type}))
         {
            if (res.val().kind() != symbol_type::e_class)
            {
               m_errors.push_back(
                  {.type = front::parse_error_type::e_semantic_error,
                   .pos = var->location(),
                   .lexeme = fmt::format(
                      "identifier '{}' to the left of `operator.` is not a class", var->lexeme())});
            }
         }
         else
         {
            m_errors.push_back(
               {.type = front::parse_error_type::e_semantic_error,
                .pos = var->location(),
                .lexeme = fmt::format("identifier '{}' to the left of `operator.` is not a class",
                                      var->lexeme())});
         }
      }

      if (const auto* dot_operator = to<ast::dot_op>(left))
      {
         return handle_dot_operator(dot_operator, tables);
      }

      if (const auto* func = to<ast::func_expr>(right))
      {
      }

      if (const auto* var = to<ast::var_expr>(right))
      {
         return handle_var_expr(var, tables);
      }

      return {};
   }

   auto type_checking_visitor::handle_function_expr(const ast::func_expr* func) -> std::string
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
                  {.type = front::parse_error_type::e_semantic_error,
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
                  {.type = front::parse_error_type::e_semantic_error,
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
         m_errors.push_back({.type = front::parse_error_type::e_semantic_error,
                             .pos = func->location(),
                             .lexeme = fmt::format("undeclared function '{}' called",
                                                   func->lexeme(), func->location().line)});

         return "";
      }
   }
   auto type_checking_visitor::handle_dot_operator(const ast::dot_op* dot) -> std::string
   {
      const auto& left = dot->children()[0];
      [[maybe_unused]] const auto& right = dot->children()[1];

      // Path only taken for free functions
      if (const auto* func = to<ast::func_expr>(left))
      {
         const auto return_type = handle_function_expr(func);

         if (const auto result = mp_root->lookup(return_type); !result)
         {
            // error
         }
      }

      if (const auto* var = to<ast::var_expr>(left))
      {
         // const auto var_name = var_expr->lexeme();
      }

      if (const auto* dot_operator = to<ast::dot_op>(left))
      {
         handle_dot_operator(dot_operator);
      }

      // dot_op has 2 children. Check if first child is not declared, error
      // if second child does not belong to class, error

      return "";
   }
} // namespace munster
