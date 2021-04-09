#include <toy_compiler/munster/ast/node.hpp>

namespace munster::ast
{
   auto node_factory(front::sem::action type, const front::lex_item& item,
                     std::vector<node_ptr>& recs) -> node_ptr;
}; // namespace munster::ast
