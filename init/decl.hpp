
#ifndef DECL_HPP
#define DECL_HPP

#include "prelude.hpp"
#include "symbol.hpp"


struct Decl
{
  Symbol const* first;
  Type const*   second;
};



// A variable declaration.
struct Variable_decl : Decl
{
  Expr const* third;
};


#endif
