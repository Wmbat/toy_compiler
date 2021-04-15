#include <toy_compiler/munster/ast/decl/func_head_decl.hpp>

#include <toy_compiler/munster/ast/decl/array_decl.hpp>
#include <toy_compiler/munster/ast/decl/variable_decl.hpp>

#include <toy_compiler/munster/ast/utility.hpp>

namespace munster::ast
{
   func_head_decl::func_head_decl(node_ptr location, node_ptr id, node_ptr class_method,
                                  node_ptr func_param, node_ptr return_type) :
      decl{std::string{id->lexeme()}, location->location()},
      m_return_type{return_type->lexeme()}
   {
      if (!std::empty(func_param->children()))
      {
         for (const node_ptr& node : func_param->children())
         {
            const auto* var = dynamic_cast<variable_decl*>(node.get());

            if (std::empty(var->children()))
            {
               std::string type{var->type()};

               m_params.emplace_back(type);
            }
            else
            {
               const auto& vars = var->children()[0];

               std::string type{var->type()};
               for (const auto& test : vars->children())
               {
                  const auto* var = dynamic_cast<array_decl*>(test.get());
                  type += fmt::format("[{}]", var->lexeme());
               }

               m_params.emplace_back(type);
            }
         }

         make_child(std::move(func_param));
      }

      if (class_method)
      {
         m_class = std::string{class_method->lexeme()};
      }
   }

   void func_head_decl::accept(visitor_variant& visitor) const
   {
      for (const auto& child : children())
      {
         child->accept(visitor);
      }

      visit_node(visitor, *this);
   }

   auto func_head_decl::class_name() const -> std::optional<std::string_view>
   {
      if (m_class)
      {
         return m_class.value();
      }

      return std::nullopt;
   }
   auto func_head_decl::return_type() const -> std::string_view { return m_return_type; }
   auto func_head_decl::params() const -> std::span<const std::string> { return m_params; }
   auto func_head_decl::to_string() const -> std::string
   {
      std::string params = "";
      for (const auto& param : m_params)
      {
         params += param;
         params += ", ";
      }

      if (m_class)
      {
         return fmt::format("func_head_decl <line:{}, col:{}> {}::{} '{} ({})'", location().line,
                            location().column, lexeme(), m_class.value(), m_return_type,
                            params.substr(0, std::size(params) - 2));
      }
      else
      {
         return fmt::format("func_head_decl <line:{}, col:{}> {} '{} ({})'", location().line,
                            location().column, lexeme(), m_return_type,
                            params.substr(0, std::size(params) - 2));
      }
   }
} // namespace munster::ast
