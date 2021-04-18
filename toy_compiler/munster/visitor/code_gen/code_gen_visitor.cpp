#include <toy_compiler/munster/visitor/code_gen/code_gen_visitor.hpp>

#include <toy_compiler/munster/ast/decl/main_decl.hpp>
#include <toy_compiler/munster/ast/decl/variable_decl.hpp>
#include <toy_compiler/munster/ast/expr/float_expr.hpp>
#include <toy_compiler/munster/ast/expr/integer_expr.hpp>
#include <toy_compiler/munster/ast/expr/var_expr.hpp>
#include <toy_compiler/munster/ast/op/add_op.hpp>
#include <toy_compiler/munster/ast/op/assign_op.hpp>
#include <toy_compiler/munster/ast/op/mult_op.hpp>
#include <toy_compiler/munster/ast/op/rel_op.hpp>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/tail.hpp>
#include <range/v3/view/transform.hpp>

namespace rv = ranges::views;

namespace munster
{
   code_gen_visitor::code_gen_visitor(symbol_table* p_root) : mp_root{p_root} {}

   void code_gen_visitor::place_marker(std::string marker)
   {
      m_blocks.push_back(std::move(marker));
   }

   auto code_gen_visitor::moon_code() -> std::string_view { return moon_code_output; }

   namespace
   {
      auto pop_back(std::vector<std::string>& stack) -> std::string
      {
         auto temp = std::move(*(std::end(stack) - 1));
         stack.pop_back();
         return temp;
      }

      auto is_temp(const symbol& symbol) -> bool
      {
         return symbol.kind() == symbol_type::e_temporary;
      }

      auto trim_leading_whitespaces(std::string_view str) -> std::string_view
      {
         const std::size_t first = str.find_first_not_of(' ');

         if (first == std::string::npos)
         {
            return str;
         }
         else
         {
            return str.substr(first, std::size(str));
         }
      }
   } // namespace

   static auto is_var(const symbol& symbol) -> bool
   {
      return symbol.kind() == symbol_type::e_variable;
   }
   static auto is_integer_lit(const symbol& symbol) -> bool
   {
      return symbol.kind() == symbol_type::e_integer_lit;
   }
   static auto is_float_lit(const symbol& symbol) -> bool
   {
      return symbol.kind() == symbol_type::e_float_lit;
   }

   static auto convert_add_op(std::string_view value) -> std::string
   {
      if (value == "+")
      {
         return "add";
      }
      else if (value == "-")
      {
         return "sub";
      }
      else
      {
         return "or";
      }
   }
   static auto convert_mult_op(std::string_view value) -> std::string
   {
      if (value == "*")
      {
         return "mul";
      }
      else if (value == "/")
      {
         return "div";
      }
      else
      {
         return "and";
      }
   }
   static auto convert_rel_op(std::string_view value) -> std::string
   {
      if (value == "==")
      {
         return "ceq";
      }
      else if (value == "!=")
      {
         return "cne";
      }
      else if (value == "<")
      {
         return "clt";
      }
      else if (value == ">")
      {
         return "cgt";
      }
      else if (value == "<=")
      {
         return "cle";
      }
      else
      {
         return "cge";
      }
   }

   void code_gen_visitor::visit(const ast::translation_unit_decl&) {}

   void code_gen_visitor::visit(const ast::compound_class_decl&) {}
   void code_gen_visitor::visit(const ast::class_decl&) {}
   void code_gen_visitor::visit(const ast::compound_inheritance_decl&) {}
   void code_gen_visitor::visit(const ast::inheritance_decl&) {}
   void code_gen_visitor::visit(const ast::compound_member_decl&) {}
   void code_gen_visitor::visit(const ast::member_func_decl&) {}
   void code_gen_visitor::visit(const ast::member_var_decl&) {}

   void code_gen_visitor::visit(const ast::compound_function_decl&) {}
   void code_gen_visitor::visit(const ast::func_decl&) {}
   void code_gen_visitor::visit(const ast::func_head_decl&) {}
   void code_gen_visitor::visit(const ast::func_body_decl&) {}
   void code_gen_visitor::visit(const ast::stmt_block_decl&)
   {
      const auto reverse_view = m_blocks | rv::reverse;
      const auto it = ranges::find(reverse_view, std::string{"stmt_block"});

      const std::int64_t dist = std::distance(it, std::end(reverse_view));
      const std::span range{std::begin(m_blocks) + dist, std::end(m_blocks)};
      const std::string block = ranges::accumulate(range, std::string{});

      m_blocks.erase(std::begin(m_blocks) + dist - 1, std::end(m_blocks));

      const std::size_t first = block.find_first_not_of(' ');

      if (first == std::string::npos)
      {
         m_blocks.emplace_back(block);
      }
      else
      {
         m_blocks.push_back(block.substr(first, std::size(block)));
      }
   }

   void code_gen_visitor::visit(const ast::main_decl& node)
   {
      moon_code_output += fmt::format("{}{}\n", spacing, "entry");
      moon_code_output += fmt::format("{}{}\n\n", spacing, "addi r14, r0, topaddr");

      for (const auto& block : m_blocks)
      {
         moon_code_output += block;
      }

      moon_code_output += fmt::format("{}hlt\n\n", spacing);

      const std::string name{node.lexeme()};
      if (const auto res = mp_root->lookup(name))
      {
         symbol_table* p_table = res.val().link();
         for (auto& symbol : p_table->symbols() | rv::values | rv::filter(is_var))
         {
            std::string_view var_name{symbol.name()};

            const std::string label{fmt::format("{}_{}", name, var_name)};

            moon_code_output += fmt::format("{:<30} {} {}\n", label, "res", symbol.size());
         }

         for (auto& symbol : p_table->symbols() | rv::values | rv::filter(is_temp))
         {
            std::string_view var_name{symbol.name()};

            const std::string label{fmt::format("{}_{}", name, var_name)};

            moon_code_output += fmt::format("{:<30} {} {}\n", label, "res", symbol.size());
         }
      }
   }

   void code_gen_visitor::visit(const ast::compound_params_decl&) {}
   void code_gen_visitor::visit(const ast::compound_variable_decl&)
   {
      if (std::empty(m_tables))
      {
         m_tables.push_back(mp_root->lookup("main").val().link());
      }
   }
   void code_gen_visitor::visit(const ast::variable_decl&) {}
   void code_gen_visitor::visit(const ast::compound_array_decl&) {}
   void code_gen_visitor::visit(const ast::array_decl&) {}

   void code_gen_visitor::visit(const ast::func_expr&) {}
   void code_gen_visitor::visit(const ast::var_expr& node)
   {
      const std::string var_name{node.lexeme()};

      if (std::size(m_tables) == 1u)
      {
         symbol_table* func_table = *(std::end(m_tables) - 1);

         if (const auto res = func_table->lookup(var_name))
         {
            m_symbols.push_back(&res.val());
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

         m_symbols.push_back(p_symbol);
      }
   }
   void code_gen_visitor::visit(const ast::float_expr& node)
   {
      auto* func_table = m_tables.back();

      const auto name = fmt::format("float_lit_{}", m_literal_counter++);
      const auto res = func_table->insert(name,
                                          symbol{{.name = name,
                                                  .kind = symbol_type::e_float_lit,
                                                  .location = node.location(),
                                                  .size = 4,
                                                  .type = "float"}});

      m_symbols.push_back(&res.val());
   }
   void code_gen_visitor::visit(const ast::integer_expr& node)
   {
      auto* func_table = m_tables.back();

      const auto name = fmt::format("integer_lit_{}", m_literal_counter++);
      const auto res = func_table->insert(name,
                                          symbol{{.name = name,
                                                  .kind = symbol_type::e_integer_lit,
                                                  .location = node.location(),
                                                  .size = 4,
                                                  .type = "integer"}});

      m_symbols.push_back(&res.val());
   }
   void code_gen_visitor::visit(const ast::string_expr&) {}

   void code_gen_visitor::visit(const ast::compound_stmt&)
   {
      if (std::empty(m_tables))
      {
         m_tables.push_back(mp_root->lookup("main").val().link());
      }
   }
   void code_gen_visitor::visit(const ast::func_stmt&) {}
   void code_gen_visitor::visit(const ast::assign_stmt&) {}
   void code_gen_visitor::visit(const ast::if_stmt&)
   {
      symbol_table* p_table = m_tables.back();

      const symbol* p_expr_sym = m_symbols.back();

      const auto r1 = m_register_pool.pop().value();

      const auto temp_label = fmt::format("{}_{}", p_table->name(), p_expr_sym->name());
      const auto else_label = fmt::format("else_{}", m_if_counter);
      const auto end_label = fmt::format("if_end_{}", m_if_counter++);

      const auto comment = "% if statement";
      const auto load = fmt::format("{}lw {}, {}(r0) {}\n", spacing, r1, temp_label, comment);
      const auto if_cmd = fmt::format("{}bz {}, {}\n", spacing, r1, else_label);
      const auto else_block = pop_back(m_blocks);
      const auto then_block = pop_back(m_blocks);

      m_blocks.push_back(fmt::format("{}{}", load, if_cmd));
      m_blocks.push_back(fmt::format("{}{}", spacing, then_block));
      m_blocks.push_back(fmt::format("{}j {}\n", spacing, end_label));
      m_blocks.push_back(fmt::format("{:<9} {}\n", else_label, else_block));
      m_blocks.push_back(fmt::format("{}\n", end_label));

      m_register_pool.push(r1);
   }
   void code_gen_visitor::visit(const ast::while_stmt&)
   {
      symbol_table* p_table = m_tables.back();

      const symbol* p_expr_sym = m_symbols.back();

      const auto r1 = m_register_pool.pop().value();

      const auto temp_label = fmt::format("{}_{}", p_table->name(), p_expr_sym->name());
      const auto beg_while_label = fmt::format("wh_beg_{}", m_if_counter);
      const auto end_while_label = fmt::format("wh_end_{}", m_if_counter);

      const auto comment = "% while statement";
      const auto load = fmt::format("{}lw {}, {}(r0) {}\n", spacing, r1, temp_label, comment);
      const auto while_cmd = fmt::format("{}bz {}, {}\n", spacing, r1, end_while_label);
      const auto while_block = pop_back(m_blocks);

      const auto comparison = pop_back(m_blocks);

      m_blocks.push_back(
         fmt::format("{:<10}{}", beg_while_label, trim_leading_whitespaces(comparison)));
      m_blocks.push_back(fmt::format("{}{}", load, while_cmd));
      m_blocks.push_back(fmt::format("{}{}", spacing, while_block));
      m_blocks.push_back(fmt::format("{}j {}\n", spacing, beg_while_label));
      m_blocks.push_back(fmt::format("{}\n", end_while_label));

      m_register_pool.push(r1);
   }
   void code_gen_visitor::visit(const ast::read_stmt&)
   {
      if (!std::empty(m_symbols))
      {
         m_symbols.pop_back();
      }
   }
   void code_gen_visitor::visit(const ast::write_stmt&)
   {
      if (!std::empty(m_symbols))
      {
         m_symbols.pop_back();
      }
   }
   void code_gen_visitor::visit(const ast::return_stmt&)
   {
      if (!std::empty(m_symbols))
      {
         m_symbols.pop_back();
      }
   }
   void code_gen_visitor::visit(const ast::break_stmt&) {}
   void code_gen_visitor::visit(const ast::continue_stmt&) {}

   void code_gen_visitor::visit(const ast::dot_op&) {}
   void code_gen_visitor::visit(const ast::assign_op& node)
   {
      // TODO: remove
      if (std::size(m_symbols) < 2)
      {
         if (!std::empty(m_symbols))
         {
            m_symbols.pop_back();
         }

         return;
      }

      symbol_table* p_table = m_tables.back();

      const symbol* p_left_symbol = *(std::end(m_symbols) - 2);
      const symbol* p_right_symbol = *(std::end(m_symbols) - 1);

      const std::string left_label{fmt::format("{}_{}", p_table->name(), p_left_symbol->name())};

      const auto cmt = "% assign op";
      if (is_float_lit(*p_right_symbol) || is_integer_lit(*p_right_symbol))
      {
         const auto value = node.children()[1]->lexeme();
         const std::string sub = fmt::format("{}sub r1, r1, r1 {}\n", spacing, cmt);
         const std::string addi = fmt::format("{}addi r1, r1, {}\n", spacing, value);
         const std::string store = fmt::format("{}sw {}(r0), r1\n\n", spacing, left_label);

         m_blocks.push_back(fmt::format("{}{}{}", sub, addi, store));
      }
      else
      {
         const auto right_label = fmt::format("{}_{}", p_table->name(), p_right_symbol->name());

         const std::string load = fmt::format("{}lw r1, {}(r0) {}\n", spacing, right_label, cmt);
         const std::string store = fmt::format("{}sw {}(r0), r1\n\n", spacing, left_label);

         m_blocks.push_back(fmt::format("{}{}", load, store));
      }

      m_symbols.pop_back();
      m_symbols.pop_back();
   }
   void code_gen_visitor::visit(const ast::add_op& node)
   {
      if (std::size(m_symbols) < 2)
      {
         if (!std::empty(m_symbols))
         {
            m_symbols.pop_back();
         }

         return;
      }

      symbol_table* p_table = m_tables.back();

      const symbol* p_left_symbol = *(std::end(m_symbols) - 2);
      const symbol* p_right_symbol = *(std::end(m_symbols) - 1);

      const std::string left_label{fmt::format("{}_{}", p_table->name(), p_left_symbol->name())};
      const std::string right_label{fmt::format("{}_{}", p_table->name(), p_right_symbol->name())};

      const auto r1 = m_register_pool.pop().value();

      const auto name = fmt::format("temp{}", m_temporary_counter++);
      const auto res = p_table->insert(name,
                                       symbol{{.name = name,
                                               .kind = symbol_type::e_temporary,
                                               .location = node.location(),
                                               .size = p_right_symbol->size(),
                                               .type = std::string{p_right_symbol->type()}}});

      const auto temp_label = fmt::format("{}_{}", p_table->name(), res.val().name());

      const auto operation = convert_add_op(node.lexeme());
      const auto cmt = fmt::format("% {} op", operation);

      if (is_float_lit(*p_right_symbol) || is_integer_lit(*p_right_symbol))
      {
         if (is_float_lit(*p_left_symbol) || is_integer_lit(*p_left_symbol))
         {
            const auto addi_left = fmt::format("{0}{2}i {1}, r0, {3} {4}\n", spacing, r1, operation,
                                               node.children()[0]->lexeme(), cmt);
            const auto addi_right = fmt::format("{0}{2}i {1}, {1}, {3}\n", spacing, r1, operation,
                                                node.children()[1]->lexeme());
            /*
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
               */
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}", addi_left, addi_right, store));
         }
         else
         {
            const auto load_left =
               fmt::format("{0}lw {1}, {2}(r0) {3}\n", spacing, r1, left_label, cmt);
            const auto addi_right =
               fmt::format("{0}{2}i {1}, {1}, {3}\n", spacing, r1, convert_add_op(node.lexeme()),
                           node.children()[1]->lexeme());
            /*
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
            */
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}", load_left, addi_right, store));
         }
      }
      else
      {
         const auto r2 = m_register_pool.pop().value();

         if (is_float_lit(*p_left_symbol) || is_integer_lit(*p_left_symbol))
         {
            const auto load_right =
               fmt::format("{0}lw {1}, {2}(r0) {3}\n", spacing, r1, right_label, cmt);
            const auto addi_left =
               fmt::format("{0}{2}i {1}, {1}, {3}\n", spacing, r1, convert_add_op(node.lexeme()),
                           node.children()[0]->lexeme());
            /*
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
               */
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}", load_right, addi_left, store));
         }
         else
         {
            const auto load_left =
               fmt::format("{0}lw {1}, {2}(r0) {3}\n", spacing, r1, left_label, cmt);
            const auto load_right = fmt::format("{0}lw {1}, {2}(r0)\n", spacing, r2, right_label);
            const auto add = fmt::format("{0}{3} {1}, {1}, {2}\n", spacing, r1, r2,
                                         convert_add_op(node.lexeme()));
            /*
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
               */
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}{}", load_left, load_right, add, store));
         }

         m_register_pool.push(r2);
      }

      m_register_pool.push(r1);

      m_symbols.pop_back();
      m_symbols.pop_back();

      m_symbols.push_back(&res.val());
   }
   void code_gen_visitor::visit(const ast::mult_op& node)
   {
      if (std::size(m_symbols) < 2)
      {
         if (!std::empty(m_symbols))
         {
            m_symbols.pop_back();
         }

         return;
      }

      symbol_table* p_table = m_tables.back();

      const symbol* p_left_symbol = *(std::end(m_symbols) - 2);
      const symbol* p_right_symbol = *(std::end(m_symbols) - 1);

      const std::string left_label{fmt::format("{}_{}", p_table->name(), p_left_symbol->name())};
      const std::string right_label{fmt::format("{}_{}", p_table->name(), p_right_symbol->name())};

      const auto r1 = m_register_pool.pop().value();

      const auto name = fmt::format("temp{}", m_temporary_counter++);
      const auto res = p_table->insert(name,
                                       symbol{{.name = name,
                                               .kind = symbol_type::e_temporary,
                                               .location = node.location(),
                                               .size = p_right_symbol->size(),
                                               .type = std::string{p_right_symbol->type()}}});

      [[maybe_unused]] const auto& test = res.val();

      const auto temp_label = fmt::format("{}_{}", p_table->name(), res.val().name());

      const auto cmt = "% mult op";
      if (is_float_lit(*p_right_symbol) || is_integer_lit(*p_right_symbol))
      {
         if (is_float_lit(*p_left_symbol) || is_integer_lit(*p_left_symbol))
         {
            const auto muli_left =
               fmt::format("{0}{2}i {1}, r0, {3} {4}\n", spacing, r1,
                           convert_mult_op(node.lexeme()), node.children()[0]->lexeme(), cmt);
            const auto muli_right =
               fmt::format("{0}{2}i {1}, {1}, {3}\n", spacing, r1, convert_mult_op(node.lexeme()),
                           node.children()[1]->lexeme());
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}", muli_left, muli_right, store));
         }
         else
         {
            const auto load_left =
               fmt::format("{}lw {}, {}(r0) {}\n", spacing, r1, left_label, cmt);
            const auto mul_right =
               fmt::format("{0}{2}i {1}, {1}, {3}\n", spacing, r1, convert_mult_op(node.lexeme()),
                           node.children()[1]->lexeme());
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}", load_left, mul_right, store));
         }
      }
      else
      {
         const auto r2 = m_register_pool.pop().value();

         if (is_float_lit(*p_left_symbol) || is_integer_lit(*p_left_symbol))
         {
            const auto load_right =
               fmt::format("{}lw {}, {}(r0) {}\n", spacing, r1, right_label, cmt);
            const auto muli_left =
               fmt::format("{0}{2}i {1}, {1}, {3}\n", spacing, r1, convert_mult_op(node.lexeme()),
                           node.children()[0]->lexeme());
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}", load_right, muli_left, store));
         }
         else
         {
            const auto load_left =
               fmt::format("{}lw {}, {}(r0) {}\n", spacing, r1, left_label, cmt);
            const auto load_right = fmt::format("{0}lw {1}, {2}(r0)\n", spacing, r2, right_label);
            const auto mul = fmt::format("{0}{3} {1}, {1}, {2}\n", spacing, r1, r2,
                                         convert_mult_op(node.lexeme()));
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}{}", load_left, load_right, mul, store));
         }

         m_register_pool.push(r2);
      }

      m_register_pool.push(r1);

      m_symbols.pop_back();
      m_symbols.pop_back();

      m_symbols.push_back(&res.val());
   }
   void code_gen_visitor::visit(const ast::rel_op& node)
   {
      if (std::size(m_symbols) < 2)
      {
         if (!std::empty(m_symbols))
         {
            m_symbols.pop_back();
         }

         return;
      }

      symbol_table* p_table = m_tables.back();

      const symbol* p_left_symbol = *(std::end(m_symbols) - 2);
      const symbol* p_right_symbol = *(std::end(m_symbols) - 1);

      const std::string left_label{fmt::format("{}_{}", p_table->name(), p_left_symbol->name())};
      const std::string right_label{fmt::format("{}_{}", p_table->name(), p_right_symbol->name())};

      const auto r1 = m_register_pool.pop().value();

      const auto name = fmt::format("temp{}", m_temporary_counter++);
      const auto res = p_table->insert(name,
                                       symbol{{.name = name,
                                               .kind = symbol_type::e_temporary,
                                               .location = node.location(),
                                               .size = p_right_symbol->size(),
                                               .type = std::string{p_right_symbol->type()}}});

      const auto temp_label = fmt::format("{}_{}", p_table->name(), res.val().name());

      if (is_float_lit(*p_right_symbol) || is_integer_lit(*p_right_symbol))
      {
         if (is_float_lit(*p_left_symbol) || is_integer_lit(*p_left_symbol))
         {
            const auto reli_left =
               fmt::format("{0}{2}i {1}, r0, {3}\n", spacing, r1, convert_rel_op(node.lexeme()),
                           node.children()[0]->lexeme());
            const auto reli_right =
               fmt::format("{0}{2}i {1}, {1}, {3}\n", spacing, r1, convert_rel_op(node.lexeme()),
                           node.children()[1]->lexeme());
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}", reli_left, reli_right, store));
         }
         else
         {
            const auto load_left = fmt::format("{0}lw {1}, {2}(r0)\n", spacing, r1, left_label);
            const auto rel_right =
               fmt::format("{0}{2}i {1}, {1}, {3}\n", spacing, r1, convert_rel_op(node.lexeme()),
                           node.children()[1]->lexeme());
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}", load_left, rel_right, store));
         }
      }
      else
      {
         const auto r2 = m_register_pool.pop().value();

         if (is_float_lit(*p_left_symbol) || is_integer_lit(*p_left_symbol))
         {
            const auto load_right = fmt::format("{0}lw {1}, {2}(r0)\n", spacing, r1, right_label);
            const auto reli_left =
               fmt::format("{0}{2}i {1}, {1}, {3}\n", spacing, r1, convert_rel_op(node.lexeme()),
                           node.children()[0]->lexeme());
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}", load_right, reli_left, store));
         }
         else
         {
            const auto load_left = fmt::format("{0}lw {1}, {2}(r0)\n", spacing, r1, left_label);
            const auto load_right = fmt::format("{0}lw {1}, {2}(r0)\n", spacing, r2, right_label);
            const auto rel = fmt::format("{0}{3} {1}, {1}, {2}\n", spacing, r1, r2,
                                         convert_rel_op(node.lexeme()));
            const auto label_res =
               fmt::format("{:<30} {} {}\n", temp_label, "res", res.val().size());
            const auto store = fmt::format("{0}sw {1}(r0), {2}\n\n", spacing, temp_label, r1);

            m_blocks.push_back(fmt::format("{}{}{}{}", load_left, load_right, rel, store));
         }

         m_register_pool.push(r2);
      }

      m_register_pool.push(r1);

      m_symbols.pop_back();
      m_symbols.pop_back();

      m_symbols.push_back(&res.val());
   }
} // namespace munster
