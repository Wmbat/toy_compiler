#include <toy_compiler/front_end/ast/list_node.hpp>

#include <range/v3/view/tail.hpp>

namespace fr::ast
{
   namespace vi = ranges::views;

   static inline std::uint32_t counter = 0;

   list::list(std::vector<node_ptr>&& children) : node{{}, {}, counter++}
   {
      node* it = children.front().get();
      for (node_ptr& node : children | vi::tail)
      {
         auto temp = node.get();
         it->make_sibling(std::move(node));
         it = temp;
      }

      make_child(std::move(children.front()));
   }

   auto list::to_string_impl(sem::action_type type) const -> std::string
   {
      std::string name = fmt::format("\"{}_{}\"", magic_enum::enum_name(type), index());

      std::string output = name + fmt::format(" -> {}", child());
      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, magic_enum::enum_name(type));

      return fmt::format("{0};\n{1}", name, output);
   }

   class_decl_list::class_decl_list(std::vector<node_ptr>&& children) : list(std::move(children)) {}

   auto class_decl_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::class_decl_list);
   }

   inheritance_declaration_list::inheritance_declaration_list(std::vector<node_ptr>&& children) :
      list{std::move(children)}
   {}

   auto inheritance_declaration_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::inheritance_decl_list);
   }

   class_body_declaration_list::class_body_declaration_list(std::vector<node_ptr>&& children) :
      list{std::move(children)}
   {}

   auto class_body_declaration_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::class_body_decl_list);
   }

   function_list::function_list(std::vector<node_ptr>&& children) : list{std::move(children)} {}

   auto function_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::func_def_list);
   }

   function_param_decl_list::function_param_decl_list(std::vector<node_ptr>&& children) :
      list{std::move(children)}
   {}

   auto function_param_decl_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::function_param_decl_list);
   }

   statement_list::statement_list(std::vector<node_ptr>&& children) : list{std::move(children)} {}

   auto statement_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::statement_list);
   }

   variable_declaration_list::variable_declaration_list(std::vector<node_ptr>&& children) :
      list{std::move(children)}
   {}

   auto variable_declaration_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::var_decl_list);
   }

   array_size_statement_list::array_size_statement_list(std::vector<node_ptr>&& children) :
      list{std::move(children)}
   {}

   auto array_size_statement_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::array_size_stmt_list);
   }

   arith_expr_list::arith_expr_list(std::vector<node_ptr>&& children) : list{std::move(children)} {}

   auto arith_expr_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::arith_expr_list);
   }

   term_tail_expr_list::term_tail_expr_list(std::vector<node_ptr>&& children) :
      list{std::move(children)}
   {}

   auto term_tail_expr_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::term_tail_expr_list);
   }

   factor_expr_list::factor_expr_list(std::vector<node_ptr>&& children) : list{std::move(children)}
   {}

   auto factor_expr_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::term_tail_expr_list);
   }

   idnest_list::idnest_list(std::vector<node_ptr>&& children) : list{std::move(children)} {}

   auto idnest_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::idnest_list);
   }

   indice_list::indice_list(std::vector<node_ptr>&& children) : list{std::move(children)} {}

   auto indice_list::to_string() const -> std::string
   {
      return to_string_impl(sem::action_type::indice_list);
   }
} // namespace fr::ast
