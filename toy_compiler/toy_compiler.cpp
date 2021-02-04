#include <toy_compiler/core/application.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/drop.hpp>

namespace views = ranges::views;

constexpr std::size_t max_expected_args = 5;

auto main(int argc, char** argv) -> int
{
   using args_array = crl::small_dynamic_array<std::string_view, max_expected_args>;

   // clang-format off
   args_array arguments = std::span<char*>{argv, static_cast<std::size_t>(argc)} 
      | views::drop(1) // first argument is program name.
      | ranges::to<args_array>;
   // clang-format on 

   application app{arguments};
}
