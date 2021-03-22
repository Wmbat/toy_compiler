#include <toy_compiler/front_end/ast_bis/function_decl.hpp>

#include <toy_compiler/front_end/ast_bis/compound_stmt.hpp>

#include <range/v3/view/tail.hpp>
#include <utility>

namespace front::ast
{
   namespace vi = ranges::views;

   compound_function_decl::compound_function_decl(std::vector<node_ptr>&& func_decls)
   {
      if (std::size(func_decls) > 0)
      {
         node* it = func_decls.front().get();
         for (node_ptr& node : func_decls | vi::tail)
         {
            assert(dynamic_cast<function_decl*>(it)); // NOLINT

            auto temp = node.get();
            it->make_sibling(std::move(node));
            it = temp;
         }

         make_child(std::move(func_decls.front()));
      }
   }

   function_decl::function_decl(node_ptr function_head, node_ptr function_body)
   {
      assert(dynamic_cast<function_head_decl*>(function_head.get())); // NOLINT
      assert(dynamic_cast<function_body_decl*>(function_body.get())); // NOLINT

      function_head->make_sibling(std::move(function_body));
      make_child(std::move(function_head));
   }

   function_head_decl::function_head_decl(node_ptr location, node_ptr id, node_ptr class_method,
                                          node_ptr function_param, node_ptr return_type) :
      decl{std::string{id->lexeme()}, location->location()},
      m_return_type{return_type->lexeme()}
   {
      if (function_param->child())
      {
         std::vector<std::string> params;

         node* it = function_param->child().get();
         while (it)
         {
            params.emplace_back(dynamic_cast<variable_decl*>(it)->type());
            it = it->sibling().get();
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
      if (statements->child())
      {
         if (variables->child())
         {
            variables->make_sibling(std::move(statements));
            make_child(std::move(variables));
         }
         else
         {
            make_child(std::move(statements));
         }
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
