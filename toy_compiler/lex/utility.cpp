#include <toy_compiler/lex/utility.hpp>

#include <range/v3/algorithm/find.hpp>

namespace lex
{
   auto to_literal(const std::string_view data) -> std::string
   {
      std::string res{data};

      {
         std::size_t start_pos = 0;
         while ((start_pos = res.find('\n', start_pos)) != std::string::npos)
         {
            res.replace(start_pos, 1, "\\n");
            start_pos += std::size("\\n");
         }
      }

      {
         std::size_t start_pos = 0;
         while ((start_pos = res.find('\r', start_pos)) != std::string::npos)
         {
            res.replace(start_pos, 1, "\\r");
            start_pos += std::size("\\r");
         }
      }

      return res;
   }
} // namespace lex
