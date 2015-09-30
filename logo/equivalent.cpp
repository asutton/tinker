
#include "equivalent.hpp"
#include "ast.hpp"


bool
is_equivalent(Atom const* a, Atom const* b)
{
  return a->symbol() == b->symbol();
}


bool
is_equivalent(And const* a, And const* b)
{
  return is_equivalent(a->first, b->first)
      && is_equivalent(a->second, b->second);
}


// TODO: Make this generic.
bool
is_equivalent(Or const* a, Or const* b)
{
  return is_equivalent(a->first, b->first)
      && is_equivalent(a->second, b->second);
}


bool
is_equivalent(Prop const* a, Prop const* b)
{
  if (typeid(*a) != typeid(*b))
    return false;

  struct Fn
  {
    Fn(Prop const* b)
      : b(b)
    { }

    bool operator()(Atom const* a) const 
    { 
      return is_equivalent(a, cast<Atom>(b)); 
    }

    bool operator()(And const* a) const 
    { 
      return is_equivalent(a, cast<And>(b)); 
    }

    bool operator()(Or const* a) const 
    { 
      return is_equivalent(a, cast<Or>(b)); 
    }
    
    Prop const* b;
  };

  return apply(a, Fn{b});
}
