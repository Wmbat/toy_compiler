#include <toy_compiler/front_end/ast/node/node.hpp>

namespace front::ast
{
   auto node_factory(sem::action type, const lex_item& item, std::vector<node_ptr>& recs)
      -> node_ptr;
}; // namespace front::ast
