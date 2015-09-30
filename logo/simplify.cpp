
#include "simplify.hpp"
#include "equivalent.hpp"
#include "ast.hpp"


// An atom cannot be simplified.
Prop const* 
simplify(Atom const* p)
{
  return p;
}


Prop const*
simplify(And const* p)
{
  Prop const* p1 = simplify(p->left());
  Prop const* p2 = simplify(p->right());
  
  // idempotence: p and p <=> p
  if (is_equivalent(p1, p2))
    return p1;

  // absorption: p and (p or q) <=> p
  if (Or const* p3 = as<Or>(p2)) {
    // p and (p or q)
    if (is_equivalent(p1, p3->left()))
      return p1;    
    // p and (q or p)
    if (is_equivalent(p1, p3->right()))
      return p1;
  } else if (Or const* p3 = as<Or>(p1)) {
    // (p or q) and p
    if (is_equivalent(p2, p3->left()))
      return p2;
    // (q or p) and p
    if (is_equivalent(p2, p3->right()))
      return p2;
  }

  // contraction: p and (p and q) <=> p and q
  if (And const* p3 = as<And>(p2)) {
    // p and (p and q)
    if (is_equivalent(p1, p3->left()))
      return p3;
    // p and (q and p)
    if (is_equivalent(p1, p3->right()))
      return p3;
  } else if (And const* p3 = as<And>(p1)) {
    // (p and q) and p
    if (is_equivalent(p2, p3->left()))
      return p3;
    // (q and p) and p
    if (is_equivalent(p2, p3->right()))
      return p3;
  }
  
  return new And(p1, p2);
}


// FIXME: Handle precedence.
Prop const*
simplify(Or const* p)
{
  Prop const* p1 = simplify(p->left());
  Prop const* p2 = simplify(p->right());
  
  // idempotence: p or p <=> p
  if (is_equivalent(p1, p2))
    return p1;

  // absorption: p or (p and q) <=> p
  //
  // Because 'and' and 'or' are comutative, we need to 
  // check a bunch of different combinations.
  if (And const* p3 = as<And>(p2)) {
    // p or (p and q)
    if (is_equivalent(p1, p3->left()))
      return p1;    
    // p or (q and p)
    if (is_equivalent(p1, p3->right()))
      return p1;
  } else if (And const* p3 = as<And>(p1)) {
    // (p and q) or p
    if (is_equivalent(p2, p3->left()))
      return p2;    
    // (q and p) or p
    if (is_equivalent(p2, p3->right()))
      return p2;
  }

  // contraction: p or (p or q) <=> p or q
  if (Or const* p3 = as<Or>(p2)) {
    // p or (p or q)
    if (is_equivalent(p1, p3->left()))
      return p3;
    // p or (q or p)
    if (is_equivalent(p1, p3->right()))
      return p3;
  } else if (Or const* p3 = as<Or>(p1)) {
    // (p or q) or p
    if (is_equivalent(p2, p3->left()))
      return p3;
    // (q or p) or p
    if (is_equivalent(p2, p3->right()))
      return p3;
  }

  return new Or(p1, p2);
}


Prop const* 
simplify(Prop const* p)
{
  struct Fn
  {
    Prop const* operator()(Atom const* p) const { return simplify(p); }
    Prop const* operator()(And const* p) const { return simplify(p); }
    Prop const* operator()(Or const* p) const { return simplify(p); }
  };
  
  return apply(p, Fn());
}
