
#ifndef TYPE_HPP
#define TYPE_HPP

#include "prelude.hpp"


struct Type_visitor
{
  virtual void visit(Boolean_type const*) = 0;
  virtual void visit(Integer_type const*) = 0;
  virtual void visit(Array_type const*) = 0;
  virtual void visit(Tuple_type const*) = 0;
};


// The base class of all types in the language.
struct Type
{
  virtual ~Type() { }
  virtual void accept(Type_visitor&) const = 0;
};


// Represents the set of two values (true and false).
struct Boolean_type : Type
{
  void accept(Type_visitor& v) const { return v.visit(this); }
};


// Represents the set of integer values.
struct Integer_type : Type
{
  void accept(Type_visitor& v) const { return v.visit(this); }
};


// A sequence of n objects of the same type.
//
//    t[e]
struct Array_type : Type
{
  void accept(Type_visitor& v) const { return v.visit(this); }

  Type const* type() const { return first; }
  Expr const* extent() const { return second; }

  Type const* first;
  Expr const* second;
};


// A fixed-length sequence of objects of
// various type.
//
//    {t1, ..., tn}
struct Tuple_type
{
  void accept(Type_visitor& v) { return v.visit(this); }

  Type_seq const& types() const     { return first; }
  Type const*     type(int i) const { return first[i]; }

  Type_seq first;
};



// -------------------------------------------------------------------------- //
//                              Generic visitors

template<typename F, typename R>
struct Generic_type_visitor : Type_visitor
{
  Generic_type_visitor(F fn)
    : fn(fn)
  { }
  
  void visit(Boolean_type const* t) { r = fn(t); }
  void visit(Integer_type const* t) { r = fn(t); }
  void visit(Array_type const* t) { r = fn(t); }
  void visit(Tuple_type const* t) { r = fn(t); }

  F fn;
  R r;
};


// A specialization for functions returning void.
template<typename F>
struct Generic_type_visitor<F, void> : Type_visitor
{
  Generic_type_visitor(F fn)
    : fn(fn)
  { }
  
  void visit(Boolean_type const* t) { fn(t); }
  void visit(Integer_type const* t) { fn(t); }
  void visit(Array_type const* t) { fn(t); }
  void visit(Tuple_type const* t) { fn(t); }

  F fn;
};


// Dispatch visitor to a void visitor.
template<typename F, typename R = typename std::result_of<F(Boolean_type const*)>::type>
inline typename std::enable_if<std::is_void<R>::value, void>::type
dispatch(Type const* t, F fn)
{
  Generic_type_visitor<F, void> v(fn);
  t->accept(v);
}


// Dispatch to a non-void visitor.
template<typename F, typename R = typename std::result_of<F(Boolean_type const*)>::type>
inline typename std::enable_if<!std::is_void<R>::value, R>::type
dispatch(Type const* t, F fn)
{
  Generic_type_visitor<F, R> v(fn);
  t->accept(v);
  return v.r;
}


// Apply fn to the propositoin p.
template<typename F, typename R = typename std::result_of<F(Boolean_type const*)>::type>
inline R
apply(Type const* t, F fn)
{
  return dispatch(t, fn);
}


#endif
