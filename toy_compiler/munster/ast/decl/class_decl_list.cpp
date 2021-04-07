#include <toy_compiler/munster/ast/decl/class_decl_list.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/move.hpp>
#include <range/v3/view/transform.hpp>

namespace vi = ranges::views;

namespace munster::ast
{
   class_decl_list::class_decl_list(std::vector<class_decl>&& class_decls)
   {
      // clang-format off
      make_family(class_decls 
            | vi::move 
            | vi::transform([](class_decl&& c) { return child_t{std::move(c)}; })
            | ranges::to<std::vector>);
      // clang-format on
   }
} // namespace munster::ast
