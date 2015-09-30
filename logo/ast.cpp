
#include "ast.hpp"

#include <iostream>


// -------------------------------------------------------------------------- //
//                              Pretry printing

void 
print(std::ostream& os, Atom const* p)
{
  os << p->symbol()->spelling();
}


// FIXME: Handle precedence.
void
print(std::ostream& os, And const* p)
{
  os << '(' << p->left() << ')'<< " and " << '(' << p->right() << ')';
}


// FIXME: Handle precedence.
void
print(std::ostream& os, Or const* p)
{
  os << '(' << p->left() << ')'<< " or " << '(' << p->right() << ')';
}


void 
print(std::ostream& os, Prop const* p)
{
  struct Fn
  {
    Fn(std::ostream& os)
      : os(os)
    { }

    void operator()(Atom const* p) const { print(os, p); }
    void operator()(And const* p) const { print(os, p); }
    void operator()(Or const* p) const { print(os, p); }

    std::ostream& os;
  };
  
  apply(p, Fn(os));
}


std::ostream& 
operator<<(std::ostream& os, Prop const* prop)
{
  print(os, prop);
  return os;
}

