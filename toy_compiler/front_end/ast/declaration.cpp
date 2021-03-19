#include <toy_compiler/front_end/ast/declaration.hpp>

namespace fr::ast
{
   static inline std::uint32_t decl_counter = 0; // NOLINT

   decl::decl(const source_location& location) : node{std::string{}, location, decl_counter++} {}

   auto decl::to_string_impl(const std::string& n) const -> std::string
   {
      std::string name = fmt::format("\"{}_{}\"", n, index());
      std::string output = name + fmt::format(" -> {}", child());

      const node* temp = child().get();
      while (temp->sibling())
      {
         output += name + fmt::format(" -> {}", temp->sibling());
         temp = temp->sibling().get();
      }

      output += fmt::format("{} [label={}]\n", name, n);

      return fmt::format("{};\n{}", name, output);
   }

   variable_decl::variable_decl(node_ptr type, node_ptr value, node_ptr array_size_list,
                                const source_location& location) :
      decl{location}
   {
      value->make_sibling(std::move(array_size_list));
      type->make_sibling(std::move(value));

      make_child(std::move(type));
   }

   auto variable_decl::to_string() const -> std::string { return to_string_impl("variable_decl"); }

   function_decl::function_decl(node_ptr id, node_ptr function_param_declaration,
                                node_ptr return_type_declaration, const source_location& location) :
      decl{location}
   {
      function_param_declaration->make_sibling(std::move(return_type_declaration));
      id->make_sibling(std::move(function_param_declaration));
      make_child(std::move(id));
   }

   auto function_decl::to_string() const -> std::string { return to_string_impl("function_decl"); }

   function_param_decl::function_param_decl(node_ptr type, node_ptr id, node_ptr array_size_list,
                                            const source_location& location) :
      decl{location}
   {
      id->make_sibling(std::move(array_size_list));
      type->make_sibling(std::move(id));
      make_child(std::move(type));
   }

   auto function_param_decl::to_string() const -> std::string
   {
      return to_string_impl("function_param_decl");
   }

   return_type_decl::return_type_decl(node_ptr id, const source_location& location) : decl{location}
   {
      make_child(std::move(id));
   }

   auto return_type_decl::to_string() const -> std::string
   {
      return to_string_impl("return_type_decl");
   }

   class_decl::class_decl(node_ptr id, node_ptr inheritance_decl_list,
                          node_ptr class_body_decl_list, const source_location& location) :
      decl{location}
   {
      inheritance_decl_list->make_sibling(std::move(class_body_decl_list));
      id->make_sibling(std::move(inheritance_decl_list));

      make_child(std::move(id));
   }

   auto class_decl::to_string() const -> std::string { return to_string_impl("class_decl"); }

   class_body_decl::class_body_decl(node_ptr visibility_decl, node_ptr member_decl,
                                    const source_location& location) :
      decl{location}
   {
      visibility_decl->make_sibling(std::move(member_decl));
      make_child(std::move(visibility_decl));
   }

   auto class_body_decl::to_string() const -> std::string
   {
      return to_string_impl("class_body_decl");
   }

   visibility_decl::visibility_decl(node_ptr value, const source_location& location) :
      decl{location}
   {
      make_child(std::move(value));
   }

   auto visibility_decl::to_string() const -> std::string
   {
      return to_string_impl("visibility_decl");
   }

   member_decl::member_decl(node_ptr var_or_func_decl, const source_location& location) :
      decl{location}
   {
      make_child(std::move(var_or_func_decl));
   }

   auto member_decl::to_string() const -> std::string { return to_string_impl("member_decl"); }

   inheritance_decl::inheritance_decl(node_ptr id, const source_location& location) : decl{location}
   {
      make_child(std::move(id));
   }

   auto inheritance_decl::to_string() const -> std::string
   {
      return to_string_impl("inheritance_decl");
   }

   idnest_decl::idnest_decl(node_ptr indice_list, node_ptr id, const source_location& location) :
      decl{location}
   {
      indice_list->make_sibling(std::move(id));
      make_child(std::move(indice_list));
   };

   auto idnest_decl::to_string() const -> std::string { return to_string_impl("idnest_decl"); }

   indice_decl::indice_decl(node_ptr expr, const source_location& location) : decl{location}
   {
      make_child(std::move(expr));
   }

   auto indice_decl::to_string() const -> std::string { return to_string_impl("indice_decl"); }

} // namespace fr::ast
