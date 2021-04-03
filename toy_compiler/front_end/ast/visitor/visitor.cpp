#include <map>
#include <toy_compiler/front_end/ast/visitor/visitor.hpp>

#include <fmt/color.h>

#include <range/v3/view/transform.hpp>

namespace front::ast
{
   namespace vi = ranges::views;

   auto pop_back(std::vector<std::unique_ptr<symbol_table>>& stack) -> std::unique_ptr<symbol_table>
   {
      auto temp = std::move(*(std::end(stack) - 1));
      stack.pop_back();
      return temp;
   }

   template <typename Any>
   auto to(const node_ptr& node) -> Any*
   {
      return dynamic_cast<Any*>(node.get());
   }

   void visitor::operator()(const translation_unit_decl* tl)
   {
      auto table = std::make_unique<symbol_table>("translation unit");

      for (const auto& node : tl->children()[0]->children())
      {
         auto* value = dynamic_cast<class_decl*>(node.get());

         const std::string name{value->lexeme()};

         std::iter_swap(ranges::find(m_tables, name, &symbol_table::name), std::end(m_tables) - 1);

         table->insert(name, symbol{name, symbol_type::e_class, "", pop_back(m_tables)});
      }

      /*
      for (const auto& node : tl.children()[1]->children())
      {
      }
      */

      m_tables.push_back(std::move(table));
   }
   void visitor::operator()(const compound_class_decl* /*ccd*/) {}
   void visitor::operator()(const class_decl* cd)
   {
      const std::string table_name{cd->lexeme()};
      auto table = std::make_unique<symbol_table>(table_name);

      for (const auto& node : cd->children())
      {
         if (dynamic_cast<compound_inheritance_decl*>(node.get()))
         {
            for (const auto* inheritance : node->children() | vi::transform(to<inheritance_decl>))
            {
               const std::string name{inheritance->lexeme()};
               table->insert(name, symbol{name, symbol_type::e_inheritance, "", {}});
            }
         }

         if (dynamic_cast<compound_member_decl*>(node.get()))
         {
            std::map<std::string_view, int> duplicates_counter;

            for (const auto* member : node->children() | vi::transform(to<member_decl>))
            {
               const auto& child = member->children()[0];

               const bool is_func = dynamic_cast<member_function_decl*>(child.get());

               if (is_func)
               {
                  const auto* func = to<member_function_decl>(child);

                  const std::string name{func->lexeme()};
                  const auto type = fmt::format("{} '{} ({})'", member->visibility(),
                                                func->return_type(), func->params_string());
                  const auto key = fmt::format("{} '{} ({})'", func->lexeme(), func->return_type(),
                                               func->params_string());

                  auto result =
                     table->insert(key, symbol{name, symbol_type::e_member_function, type});

                  if (!result.is_inserted())
                  {
                     const parse_error err{
                        .type = parse_error_type::e_semantic_error,
                        .pos = func->location(),
                        .lexeme = fmt::format(
                           "function '{}' of type '{} ({})' already defined in class '{}'",
                           func->lexeme(), func->return_type(), func->params_string(),
                           cd->lexeme())};
                     m_errors.push_back(err);
                  }
                  else
                  {
                     const auto dup = duplicates_counter.insert({func->lexeme(), 1});
                     if (dup.second == false)
                     {
                        const parse_error err{.type = parse_error_type::e_semantic_warning,
                                              .pos = func->location(),
                                              .lexeme =
                                                 fmt::format("function '{}' of type '{} ({})' "
                                                             "defined in class '{}' is an overload",
                                                             func->lexeme(), func->return_type(),
                                                             func->params_string(), cd->lexeme())};
                        m_errors.push_back(err);
                     }
                  }
               }
               else
               {
                  const auto* var = to<variable_decl>(child);

                  const std::string name{var->lexeme()};
                  const auto type = fmt::format("{} {}", member->visibility(), var->type());

                  const auto result =
                     table->insert(name, symbol{name, symbol_type::e_member_variable, type});

                  if (!result.is_inserted())
                  {
                     const parse_error err{
                        .type = parse_error_type::e_semantic_error,
                        .pos = var->location(),
                        .lexeme = fmt::format("variable '{}' already defined in class '{}'",
                                              var->lexeme(), cd->lexeme())};
                     m_errors.push_back(err);
                  }
               }
            }
         }
      }

      m_tables.push_back(std::move(table));
   }
   void visitor::operator()(const compound_function_decl* /*cfd*/) {}

   auto visitor::get_errors() const -> const std::vector<parse_error>& { return m_errors; }
} // namespace front::ast
