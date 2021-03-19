#pragma once

#include <toy_compiler/front_end/ast_bis/node.hpp>

namespace fr::ast_bis
{
   class stmt : public node
   {
   };

   class assign_stmt : public stmt
   {
   };

   class if_stmt : public stmt
   {
   };

   class while_stmt : public stmt
   {
   };

   class read_stmt : public stmt
   {
   };

   class write_stmt : public stmt
   {
   };

   class return_stmt : public stmt
   {
   };

   class break_stmt : public stmt
   {
   };

   class continue_stmt : public stmt
   {
   };
} // namespace fr::ast_bis
