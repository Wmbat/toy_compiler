#include <toy_compiler/munster/visitor/symbol_table/memory_size_visitor.hpp>

#include <toy_compiler/munster/ast/decl/class_decl.hpp>
#include <toy_compiler/munster/ast/decl/func_head_decl.hpp>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/transform.hpp>

namespace rv = ranges::views;

namespace munster
{
   auto is_mem_var(const symbol& symbol) -> bool
   {
      return symbol.kind() == symbol_type::e_member_variable;
   }

   auto is_pod(std::string_view type) -> bool { return type == "integer" || type == "float"; }

   memory_size_visitor::memory_size_visitor(symbol_table* p_root) : mp_root{p_root} {}

   void memory_size_visitor::visit(const ast::translation_unit_decl&) {}

   void memory_size_visitor::visit(const ast::compound_class_decl&) {}
   void memory_size_visitor::visit(const ast::class_decl& node)
   {
      const std::string class_name{node.lexeme()};

      if (auto res = mp_root->lookup(class_name))
      {
         symbol_table* class_table = res.val().link();

         const auto get_size = [&](const symbol& mem_var) -> std::int64_t {
            // clang-format off
            const auto type_data = mem_var.type() 
               | rv::split(' ') 
               | ranges::to<std::vector<std::string>>;
            // clang-format on

            const std::string& type = type_data.back();
            if (is_pod(type))
            {
               return 4;
            }

            if (auto res = mp_root->lookup(type))
            {
               return res.val().size();
            }

            return 0;
         };

         // clang-format off
         
         res.val().update_size(ranges::accumulate(class_table->symbols() 
               | rv::values
               | rv::filter(is_mem_var)
               | rv::transform(get_size), 0L));

         // clang-format on
      }
   }
   void memory_size_visitor::visit(const ast::compound_inheritance_decl&) {}
   void memory_size_visitor::visit(const ast::inheritance_decl&) {}
   void memory_size_visitor::visit(const ast::compound_member_decl&) {}
   void memory_size_visitor::visit(const ast::member_func_decl&) {}
   void memory_size_visitor::visit(const ast::member_var_decl&) {}

   void memory_size_visitor::visit(const ast::compound_function_decl&) {}
   void memory_size_visitor::visit(const ast::func_decl&)
   {
      symbol_table* table = m_tables.back();

      for (auto& symbol : table->symbols() | rv::values)
      {
         if (!is_pod(symbol.type()))
         {
            const std::string key{symbol.type()};
            if (auto res = mp_root->lookup(key))
            {
               symbol.update_size(res.val().size());
            }
         }
      }

      m_tables.pop_back();
   }
   void memory_size_visitor::visit(const ast::func_head_decl& node)
   {
      if (node.class_name())
      {
         const std::string class_name{node.lexeme()};
         const auto func_name = node.class_name().value();
         const auto key = fmt::format("{} '{} ({})'", func_name, node.return_type(),
                                      fmt::join(node.params(), ", "));

         if (auto class_entry_res = mp_root->lookup(class_name))
         {
            symbol_table* class_table = class_entry_res.val().link();
            if (auto func_entry_res = class_table->lookup(key))
            {
               m_tables.push_back(func_entry_res.val().link());
            }
         }
      }
      else
      {
         const auto func_name = node.lexeme();
         const auto key = fmt::format("{} '{} ({})'", func_name, node.return_type(),
                                      fmt::join(node.params(), ", "));

         if (auto func_entry_res = mp_root->lookup(key))
         {
            m_tables.push_back(func_entry_res.val().link());
         }
      }
   }
   void memory_size_visitor::visit(const ast::func_body_decl&) {}

   void memory_size_visitor::visit(const ast::main_decl&) {}

   void memory_size_visitor::visit(const ast::compound_params_decl&) {}
   void memory_size_visitor::visit(const ast::compound_variable_decl&) {}
   void memory_size_visitor::visit(const ast::variable_decl&) {}
   void memory_size_visitor::visit(const ast::compound_array_decl&) {}
   void memory_size_visitor::visit(const ast::array_decl&) {}

   void memory_size_visitor::visit(const ast::func_expr&) {}
   void memory_size_visitor::visit(const ast::var_expr&) {}
   void memory_size_visitor::visit(const ast::float_expr&) {}
   void memory_size_visitor::visit(const ast::integer_expr&) {}
   void memory_size_visitor::visit(const ast::string_expr&) {}

   void memory_size_visitor::visit(const ast::compound_stmt&) {}
   void memory_size_visitor::visit(const ast::func_stmt&) {}
   void memory_size_visitor::visit(const ast::assign_stmt&) {}
   void memory_size_visitor::visit(const ast::if_stmt&) {}
   void memory_size_visitor::visit(const ast::while_stmt&) {}
   void memory_size_visitor::visit(const ast::read_stmt&) {}
   void memory_size_visitor::visit(const ast::write_stmt&) {}
   void memory_size_visitor::visit(const ast::return_stmt&) {}
   void memory_size_visitor::visit(const ast::break_stmt&) {}
   void memory_size_visitor::visit(const ast::continue_stmt&) {}

   void memory_size_visitor::visit(const ast::dot_op&) {}
   void memory_size_visitor::visit(const ast::assign_op&) {}
   void memory_size_visitor::visit(const ast::add_op&) {}
   void memory_size_visitor::visit(const ast::mult_op&) {}
   void memory_size_visitor::visit(const ast::rel_op&) {}
} // namespace munster