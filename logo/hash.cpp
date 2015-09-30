
#include "hash.hpp"
#include "ast.hpp"

#include <boost/functional/hash.hpp>


std::size_t
hash_value(Atom const* p)
{
  std::hash<void const*> h;
  return h(p->symbol());
}


std::size_t
hash_value(And const* p)
{
  std::size_t seed;
  boost::hash_combine(seed, p->left());
  boost::hash_combine(seed, p->right());
  return seed;
}


// FIXME: Handle precedence.
std::size_t
hash_value(Or const* p)
{
  std::size_t seed;
  boost::hash_combine(seed, p->left());
  boost::hash_combine(seed, p->right());
  return seed;
}


std::size_t
hash_value(Prop const* p)
{
  struct Fn
  {
    std::size_t operator()(Atom const* p) const { return hash_value(p); }
    std::size_t operator()(And const* p) const { return hash_value(p); }
    std::size_t operator()(Or const* p) const { return hash_value(p); }
  };
  
  return apply(p, Fn());
}
