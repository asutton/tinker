
#ifndef AST_HPP
#define AST_HPP

#include "cast.hpp"
#include "symbol.hpp"

struct Prop;
struct Atom;
struct And;
struct Or;


// The visitor class.
struct Visitor
{
  virtual void visit(Atom const*) = 0;
  virtual void visit(And const*) = 0;
  virtual void visit(Or const*) = 0;
};


// The base class of all logical propositions.
struct Prop
{
  virtual ~Prop() { }
  virtual void accept(Visitor& v) const = 0;
};


// Atomic propositions.
struct Atom : Prop
{
  Atom(Symbol const* s)
    : sym_(s)
  { }

  void accept(Visitor& v) const { return v.visit(this); }

  Symbol const* symbol() const { return sym_; }

  Symbol const* sym_;
};


// A helper class.
struct Binary : Prop
{
  Binary(Prop const* e1, Prop const* e2)
    : first(e1), second(e2)
  { }

  Prop const* left() const { return first; }
  Prop const* right() const { return second; }

  Prop const* first;
  Prop const* second;
};


// Conjunction of propositions.
struct And : Binary
{
  using Binary::Binary;

  void accept(Visitor& v) const { return v.visit(this); }
};


// Disjunction of propositions.
struct Or : Binary
{
  using Binary::Binary;
  
  void accept(Visitor& v) const { return v.visit(this); }
};


// -------------------------------------------------------------------------- //
//                                Operations

void print(std::ostream&, Prop const*);

std::ostream& operator<<(std::ostream&, Prop const*);


// -------------------------------------------------------------------------- //
//                              Generic visitors

template<typename F, typename R>
struct Generic_visitor : Visitor
{
  Generic_visitor(F fn)
    : fn(fn)
  { }
  
  void visit(Atom const* p) { r = fn(p); }
  void visit(And const* p) { r = fn(p); }
  void visit(Or const* p) { r = fn(p); }

  F fn;
  R r;
};


// A specialization for functions returning void.
template<typename F>
struct Generic_visitor<F, void> : Visitor
{
  Generic_visitor(F fn)
    : fn(fn)
  { }
  
  void visit(Atom const* p) { fn(p); }
  void visit(And const* p) { fn(p); }
  void visit(Or const* p) { fn(p); }

  F fn;
};


// Dispatch visitor to a void visitor.
template<typename F, typename R = typename std::result_of<F(Atom const*)>::type>
inline typename std::enable_if<std::is_void<R>::value, void>::type
dispatch(Prop const* p, F fn)
{
  Generic_visitor<F, void> v(fn);
  p->accept(v);
}


// Dispatch to a non-void visitor.
template<typename F, typename R = typename std::result_of<F(Atom const*)>::type>
inline typename std::enable_if<!std::is_void<R>::value, R>::type
dispatch(Prop const* p, F fn)
{
  Generic_visitor<F, R> v(fn);
  p->accept(v);
  return v.r;
}


// Apply fn to the propositoin p.
template<typename F, typename R = typename std::result_of<F(Atom const*)>::type>
inline R
apply(Prop const* p, F fn)
{
  return dispatch(p, fn);
}



#endif
