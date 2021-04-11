#include <toy_compiler/munster/visitor/ast/symbol_table_visitor.hpp>

#include <toy_compiler/munster/ast/decl/class_decl.hpp>
#include <toy_compiler/munster/ast/decl/compound_inheritance_decl.hpp>
#include <toy_compiler/munster/ast/decl/compound_member_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_body_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_head_decl.hpp>
#include <toy_compiler/munster/ast/decl/inheritance_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_func_decl.hpp>
#include <toy_compiler/munster/ast/decl/member_var_decl.hpp>
#include <toy_compiler/munster/ast/decl/translation_unit_decl.hpp>

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

#include <fmt/color.h>
#include <fmt/core.h>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>

#include <map>

namespace vi = ranges::views;

namespace munster
{
   auto pop_back(std::vector<std::unique_ptr<symbol_table>>& stack) -> std::unique_ptr<symbol_table>
   {
      auto temp = std::move(*(std::end(stack) - 1));
      stack.pop_back();
      return temp;
   }

   template <typename Any>
   auto to(const ast::node_ptr& node) -> Any*
   {
      return dynamic_cast<Any*>(node.get());
   }

   void symbol_table_visitor::visit(const ast::translation_unit_decl& tl)
   {
      auto table = std::make_unique<symbol_table>("translation unit");

      for (const auto& node : tl.children()[0]->children())
      {
         auto* value = dynamic_cast<ast::class_decl*>(node.get());

         const std::string name{value->lexeme()};

         std::iter_swap(ranges::find(m_tables, name, &symbol_table::name), std::end(m_tables) - 1);

         auto result = table->insert(
            name, symbol{name, symbol_type::e_class, value->location(), "", pop_back(m_tables)});

         if (!result.is_inserted())
         {
            const front::parse_error err{
               .type = front::parse_error_type::e_semantic_error,
               .pos = value->location(),
               .lexeme = fmt::format("class '{}' already defined on line {}", value->lexeme(),
                                     result.val().location().line)};
            m_errors.push_back(err);
         }
      }

      for (const auto& node : tl.children()[1]->children())
      {
         auto* func = dynamic_cast<ast::func_decl*>(node.get());
         auto* func_head = dynamic_cast<ast::func_head_decl*>(func->children()[0].get());

         const std::string name{func_head->lexeme()};
         const auto type =
            fmt::format("{} ({})", func_head->return_type(), fmt::join(func_head->params(), ", "));
         const auto key = fmt::format("{} '{} ({})'", func_head->lexeme(), func_head->return_type(),
                                      fmt::join(func_head->params(), ", "));

         const auto it = ranges::find(m_tables, name, &symbol_table::name);
         if (it != std::end(m_tables))
         {
            std::iter_swap(it, std::end(m_tables) - 1);

            // crosscheck params

            auto result = table->insert(
               key,
               symbol{name, symbol_type::e_function, func->location(), type, pop_back(m_tables)});

            if (!result.is_inserted())
            {
               m_errors.push_back({.type = front::parse_error_type::e_semantic_error,
                                   .pos = func_head->location(),
                                   .lexeme = fmt::format("function '{}' already defined on line {}",
                                                         name, result.val().location().line)});
            }
         }
      }

      const auto* main_func = tl.children()[2].get();
      const std::string name{main_func->lexeme()};

      const auto it = ranges::find(m_tables, name, &symbol_table::name);
      if (it != std::end(m_tables))
      {
         std::iter_swap(it, std::end(m_tables) - 1);

         auto result = table->insert(
            name, symbol{name, symbol_type::e_main, main_func->location(), "", pop_back(m_tables)});

         if (!result.is_inserted())
         {
            m_errors.push_back({.type = front::parse_error_type::e_semantic_error,
                                .pos = main_func->location(),
                                .lexeme = fmt::format("function '{}' already defined on line {}",
                                                      name, result.val().location().line)});
         }
      }
      else
      {
         m_errors.push_back({.type = front::parse_error_type::e_semantic_error,
                             .pos = main_func->location(),
                             .lexeme = fmt::format("missing '{}' function", name)});
      }

      m_tables.push_back(std::move(table));
   }
   void symbol_table_visitor::visit(const ast::compound_class_decl& /*ccd*/) {}
   void symbol_table_visitor::visit(const ast::class_decl& node_t)
   {
      const std::string table_name{node_t.lexeme()};
      auto table = std::make_unique<symbol_table>(table_name);

      // clang-format off
      auto member_functions = m_symbols 
         | vi::filter([](symbol_kv& s) { 
               return s.val.kind() == symbol_type::e_member_function;
            }) 
         | vi::move;
      auto member_variables = m_symbols 
         | vi::filter([](symbol_kv& s) {
               return s.val.kind() == symbol_type::e_member_variable;
            }) 
         | vi::move;
      auto inheritance = m_symbols 
         | vi::filter([](symbol_kv& s) {
               return s.val.kind() == symbol_type::e_inheritance;
            }) 
         | vi::move;
      // clang-format on

      for (auto [key, sym] : inheritance)
      {
         const std::string name{sym.name()};
         if (ranges::find(m_tables, name, &symbol_table::name) != std::end(m_tables))
         {
            const auto location = sym.location();

            if (auto result = table->insert(key, std::move(sym)); !result.is_inserted())
            {
               const front::parse_error err{
                  .type = front::parse_error_type::e_semantic_error,
                  .pos = location,
                  .lexeme = fmt::format("inheritance declaration '{}' already defined", name,
                                        result.val().location().line)};
               m_errors.push_back(err);
            }
         }
      }

      for (auto [key, sym] : member_variables)
      {
         const auto name = sym.name();
         const auto location = sym.location();
         const auto result = table->insert(key, std::move(sym));

         if (!result.is_inserted())
         {
            const front::parse_error err{
               .type = front::parse_error_type::e_semantic_error,
               .pos = location,
               .lexeme =
                  fmt::format("variable '{}' already defined in class '{}'", name, table_name)};

            m_errors.push_back(err);
         }
      }

      std::map<std::string_view, int> duplicates_counter;
      for (auto [key, sym] : member_functions)
      {
         const auto name = sym.name();
         const auto type = sym.type();
         const auto location = sym.location();

         if (auto result = table->insert(key, std::move(sym)); !result.is_inserted())
         {
            const front::parse_error err{
               .type = front::parse_error_type::e_semantic_error,
               .pos = location,
               .lexeme = fmt::format("function '{}' of type {} already defined in class '{}'", name,
                                     type, table_name)};
            m_errors.push_back(err);
         }
         else
         {
            const auto dup = duplicates_counter.insert({name, 1});
            if (dup.second == false)
            {
               const front::parse_error err{
                  .type = front::parse_error_type::e_semantic_warning,
                  .pos = location,
                  .lexeme =
                     fmt::format("function '{}' of type {} defined in class '{}' is an overload",
                                 name, type, table_name)};
               m_errors.push_back(err);
            }
         }
      }

      /*
      for (const auto& node : node_t.children())
      {
         if (dynamic_cast<ast::compound_member_decl*>(node.get()))
         {
            for (const auto& member : node->children())
            {
               const bool is_func = dynamic_cast<ast::member_func_decl*>(member.get());

               if (is_func)
               {
                  const auto* func = to<ast::member_func_decl>(member);

                  const std::string name{func->lexeme()};
                  const auto type = fmt::format("{} '{} ({})'", func->visibility(),
                                                func->return_type(), func->params_string());
                  const auto key = fmt::format("{} '{} ({})'", func->lexeme(), func->return_type(),
                                               func->params_string());
               }
            }
         }
      }
      */

      m_symbols.clear();
      m_tables.push_back(std::move(table));
   }
   void symbol_table_visitor::visit(const ast::compound_inheritance_decl&) {}
   void symbol_table_visitor::visit(const ast::inheritance_decl& node)
   {
      const std::string name{node.lexeme()};
      m_symbols.push_back(
         {.key = name, .val = symbol{name, symbol_type::e_inheritance, node.location()}});
   }
   void symbol_table_visitor::visit(const ast::compound_member_decl&) {}
   void symbol_table_visitor::visit(const ast::member_func_decl& node)
   {
      const std::string name{node.lexeme()};
      const auto type =
         fmt::format("{} '{} ({})'", node.visibility(), node.return_type(), node.params_string());
      const auto key =
         fmt::format("{} '{} ({})'", node.lexeme(), node.return_type(), node.params_string());

      m_symbols.push_back(
         {.key = key, .val = symbol{name, symbol_type::e_member_variable, node.location(), type}});
   }
   void symbol_table_visitor::visit(const ast::member_var_decl& node)
   {
      const std::string name{node.lexeme()};
      const auto type = fmt::format("{} {}", node.visibility(), node.type());

      m_symbols.push_back(
         {.key = name, .val = symbol{name, symbol_type::e_member_variable, node.location(), type}});
   }

   void symbol_table_visitor::visit(const ast::compound_variable_decl&) {}
   void symbol_table_visitor::visit(const ast::compound_function_decl&) {}
   void symbol_table_visitor::visit(const ast::func_decl& fd)
   {
      const auto* head = to<ast::func_head_decl>(fd.children()[0]);
      const auto* body = to<ast::func_body_decl>(fd.children()[1]);

      std::string table_name;
      if (head->class_name())
      {
         table_name = fmt::format("{}::{}", head->lexeme(), head->class_name().value());
      }
      else
      {
         table_name = head->lexeme();
      }

      auto table = std::make_unique<symbol_table>(table_name);

      // Check return type

      {
         auto symbols = generate_var_symbols(head);
         for (auto symbol : symbols | vi::move)
         {
            const std::string name{symbol.name()};
            const auto result = table->insert(name, std::move(symbol));

            if (!result.is_inserted())
            {
               const front::parse_error err{
                  .type = front::parse_error_type::e_semantic_error,
                  .pos = result.val().location(),
                  .lexeme = fmt::format("Parameter '{}' already declared in function '{}'",
                                        result.val().name(), table->name())};

               m_errors.push_back(err);
            }
         }
      }

      if (!std::empty(body->children()))
      {
         const auto& child = body->children()[0];

         if (const auto* value = to<ast::compound_variable_decl>(child))
         {
            for (const auto* var : value->children() | vi::transform(to<ast::variable_decl>))
            {
               const std::string name{var->lexeme()};
               const std::string type{var->type()};

               const auto result =
                  table->insert(name, symbol(name, symbol_type::e_variable, var->location(), type));

               if (!result.is_inserted())
               {
                  const front::parse_error err{
                     .type = front::parse_error_type::e_semantic_error,
                     .pos = var->location(),
                     .lexeme = fmt::format("variable '{}' already declared on line '{}'",
                                           var->lexeme(), var->location().line)};

                  m_errors.push_back(err);
               }
            }
         }
      }

      if (head->class_name())
      {
         const auto class_name = head->class_name().value();
         const auto class_it = ranges::find(m_tables, head->lexeme(), &symbol_table::name);
         if (class_it != std::end(m_tables))
         {
            const auto& class_table = *class_it;

            const auto key = fmt::format("{} '{} ({})'", class_name, head->return_type(),
                                         fmt::join(head->params(), ", "));

            auto result = class_table->lookup(key);
            if (result)
            {
               result.val().set_table(std::move(table));
            }
            else
            {
               m_errors.push_back({.type = front::parse_error_type::e_semantic_error,
                                   .pos = head->location(),
                                   .lexeme = fmt::format("function '{}' not declared in class '{}'",
                                                         class_name, head->lexeme())});
            }
         }
         else
         {
            m_errors.push_back({.type = front::parse_error_type::e_semantic_error,
                                .pos = head->location(),
                                .lexeme = fmt::format("class '{}' not declared for function '{}'",
                                                      head->lexeme(), class_name)});
         }
      }
      else
      {
         m_tables.push_back(std::move(table));
      }
   }
   void symbol_table_visitor::visit(const ast::func_head_decl& /*fd*/) {}
   void symbol_table_visitor::visit(const ast::func_body_decl& /*fbd*/) {}
   void symbol_table_visitor::visit(const ast::variable_decl&) {}
   void symbol_table_visitor::visit(const ast::main_decl& md)
   {
      const std::string table_name{md.lexeme()};
      auto table = std::make_unique<symbol_table>(table_name);

      const auto* body = to<ast::func_body_decl>(md.children()[0]);
      if (!std::empty(body->children()))
      {
         const auto& child = body->children()[0];

         if (const auto* value = to<ast::compound_variable_decl>(child))
         {
            for (const auto* var : value->children() | vi::transform(to<ast::variable_decl>))
            {
               const std::string name{var->lexeme()};
               const std::string type{var->type()};

               const auto result =
                  table->insert(name, symbol(name, symbol_type::e_variable, var->location(), type));

               if (!result.is_inserted())
               {
                  const front::parse_error err{
                     .type = front::parse_error_type::e_semantic_error,
                     .pos = var->location(),
                     .lexeme = fmt::format("variable '{}' already declared on line '{}'",
                                           var->lexeme(), var->location().line)};

                  m_errors.push_back(err);
               }
            }
         }
      }

      m_tables.push_back(std::move(table));
   }

   void symbol_table_visitor::visit(const ast::func_expr& /*node*/) {}
   void symbol_table_visitor::visit(const ast::var_expr& /*node*/) {}
   void symbol_table_visitor::visit(const ast::float_expr& /*node*/) {}
   void symbol_table_visitor::visit(const ast::integer_expr& /*node*/) {}
   void symbol_table_visitor::visit(const ast::string_expr& /*node*/) {}

   void symbol_table_visitor::visit(const ast::compound_stmt& /*node*/) {}
   void symbol_table_visitor::visit(const ast::func_stmt& /*node*/) {}
   void symbol_table_visitor::visit(const ast::assign_stmt& /*node*/) {}
   void symbol_table_visitor::visit(const ast::if_stmt& /*node*/) {}
   void symbol_table_visitor::visit(const ast::while_stmt& /*node*/) {}
   void symbol_table_visitor::visit(const ast::read_stmt& /*node*/) {}
   void symbol_table_visitor::visit(const ast::write_stmt& /*node*/) {}
   void symbol_table_visitor::visit(const ast::return_stmt& /*node*/) {}
   void symbol_table_visitor::visit(const ast::break_stmt& /*node*/) {}
   void symbol_table_visitor::visit(const ast::continue_stmt& /*node*/) {}

   void symbol_table_visitor::visit(const ast::dot_op& /*node*/) {}
   void symbol_table_visitor::visit(const ast::assign_op& /*node*/) {}

   auto symbol_table_visitor::get_root_table() const -> symbol_table*
   {
      return m_tables.back().get();
   }

   auto symbol_table_visitor::generate_var_symbols(const ast::node* head) -> std::vector<symbol>
   {
      std::vector<symbol> symbols;
      if (!std::empty(head->children()))
      {
         const auto& params = head->children()[0];
         for (const auto* var : params->children() | vi::transform(to<ast::variable_decl>))
         {
            const std::string name{var->lexeme()};

            if (std::empty(var->children()))
            {
               const std::string type{var->type()};

               symbols.emplace_back(name, symbol_type::e_parameter, var->location(), type);
            }
            else
            {
               std::string type{var->type()};

               const auto& array_decls = var->children()[0];
               for (const auto* array :
                    array_decls->children() | vi::transform(to<ast::array_decl>))
               {
                  type += fmt::format("[{}]", array->lexeme());
               }

               symbols.emplace_back(name, symbol_type::e_parameter, var->location(), type);
            }
         }
      }

      return symbols;
   }
} // namespace munster
