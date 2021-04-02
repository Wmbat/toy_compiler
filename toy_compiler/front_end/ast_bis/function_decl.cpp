#include <toy_compiler/front_end/ast_bis/function_decl.hpp>

#include <toy_compiler/front_end/ast_bis/compound_stmt.hpp>

#include <range/v3/view/tail.hpp>
#include <utility>

namespace front::ast
{
   namespace vi = ranges::views;

   compound_function_decl::compound_function_decl(std::vector<node_ptr>&& func_decls)
   {
      make_family<function_decl>(std::move(func_decls));
   }

   function_decl::function_decl(node_ptr function_head, node_ptr function_body)
   {
      assert(dynamic_cast<function_head_decl*>(function_head.get())); // NOLINT
      assert(dynamic_cast<function_body_decl*>(function_body.get())); // NOLINT

      make_child(std::move(function_head));
      make_child(std::move(function_body));
   }

   function_head_decl::function_head_decl(node_ptr location, node_ptr id, node_ptr class_method,
                                          node_ptr function_param, node_ptr return_type) :
      decl{std::string{id->lexeme()}, location->location()},
      m_return_type{return_type->lexeme()}
   {
      if (!std::empty(function_param->children()))
      {
         std::vector<std::string> params;

         for (const node_ptr& node : function_param->children())
         {
            params.emplace_back(dynamic_cast<variable_decl*>(node.get())->type());
         }

         m_params = params;

         make_child(std::move(function_param));
      }

      if (class_method)
      {
         m_class = std::string{class_method->lexeme()};
      }
   }

   function_body_decl::function_body_decl(node_ptr variables, node_ptr statements)
   {
      if (!std::empty(variables->children()))
      {
         make_child(std::move(variables));
      }

      if (!std::empty(statements->children()))
      {
         make_child(std::move(statements));
      }
   }

   auto compound_function_decl::to_string() const -> std::string
   {
      return "compound_function_decl";
   }
   auto function_decl::to_string() const -> std::string { return "function_decl"; }
   auto function_head_decl::to_string() const -> std::string
   {
      std::string params = "";
      if (m_params)
      {
         for (const auto& param : m_params.value())
         {
            params += param;
            params += ", ";
         }
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
   auto function_body_decl::to_string() const -> std::string { return "function_body_decl"; }
} // namespace front::ast
