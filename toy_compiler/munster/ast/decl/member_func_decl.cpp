#include <toy_compiler/munster/ast/decl/member_func_decl.hpp>

#include <toy_compiler/front_end/ast_bis/declaration.hpp>

namespace munster::ast
{
   member_func_decl::member_func_decl(node_ptr visibility, node_ptr location, node_ptr id,
                                      node_ptr compound_param, node_ptr tail) :
      decl{std::string{id->lexeme()}, location->location()},
      m_return_type{tail->lexeme()}
   {
      if (visibility)
      {
         m_visibility = visibility->lexeme();
      }

      if (!std::empty(compound_param->children()))
      {
         for (const node_ptr& node : compound_param->children())
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

         make_child(std::move(compound_param));
      }
   }

   auto member_func_decl::visibility() const -> std::string_view { return m_visibility; }

   auto member_func_decl::return_type() const -> std::string_view { return m_return_type; }
   auto member_func_decl::params() const -> std::span<const std::string> { return m_params; }

   auto member_func_decl::params_string() const -> std::string
   {
      std::string params;
      for (const auto& param : m_params)
      {
         params += param;
         params += ", ";
      }

      return params.substr(0, std::size(params) - 2);
   }
   auto member_func_decl::to_string() const -> std::string
   {
      return fmt::format("member_func_decl {} {} '{} {} ({})'", location(), lexeme(), visibility(),
                         m_return_type, params_string());
   }
} // namespace munster::ast
