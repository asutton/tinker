
#ifndef VALUE_HPP
#define VALUE_HPP

// This module defines the abstract representation of
// compile-time values.

#include <cstdint>
#include <stdexcept>


struct Value;


// Integer values.
using Integer_value = std::intmax_t;


// Aggregate values.
struct Aggregate_value
{
  int    len;
  Value* elems;
};


// Enumeration of value kinds.
enum Value_kind
{
  integer_value,
  aggregate_value
};


// Internal representations of value.
//
// TODO: Add constructors.
union Value_data
{
  Value_data(Integer_value n)
    : z(n)
  { }

  Integer_value   z;
  Aggregate_value a;
};


// Visitor for values.
struct Value_visitor
{
  virtual void visit(Integer_value const&) = 0;
  virtual void visit(Aggregate_value const&) = 0;
};


// Represents a compile-time value. This is a discriminated 
// capable of representing values of many different kind.
//
// TODO: Add constructors.
struct Value
{
  Value(Integer_value n)
    : kind_(integer_value), data_(n)
  { }

  void accept(Value_visitor& v) const;

  Value_kind kind() const { return kind_; }  

  Value_kind kind_;
  Value_data data_;
};


inline void
Value::accept(Value_visitor& v) const
{
  switch (kind_) {
    case integer_value:
      return v.visit(data_.z);
    case aggregate_value:
      return v.visit(data_.a);
    default:
      throw std::runtime_error("invalid value");
  }
}



// -------------------------------------------------------------------------- //
//                              Generic visitors

template<typename F, typename R>
struct Generic_value_visitor : Value_visitor
{
  Generic_value_visitor(F fn)
    : fn(fn)
  { }
  
  void visit(Integer_value const& v) { r = fn(v); }
  void visit(Aggregate_value const& v) { r = fn(v); }

  F fn;
  R r;
};


// A specialization for functions returning void.
template<typename F>
struct Generic_value_visitor<F, void> : Value_visitor
{
  Generic_value_visitor(F fn)
    : fn(fn)
  { }
  
  void visit(Integer_value const& v) { fn(v); }
  void visit(Aggregate_value const& v) { fn(v); }

  F fn;
};


// Dispatch visitor to a void visitor.
template<typename F, typename R = typename std::result_of<F(Integer_value const&)>::type>
inline typename std::enable_if<std::is_void<R>::value, void>::type
dispatch(Value const& v, F fn)
{
  Generic_value_visitor<F, void> vis(fn);
  v.accept(vis);
}


// Dispatch to a non-void visitor.
template<typename F, typename R = typename std::result_of<F(Integer_value const&)>::type>
inline typename std::enable_if<!std::is_void<R>::value, R>::type
dispatch(Value const& v, F fn)
{
  Generic_value_visitor<F, R> vis(fn);
  v.accept(vis);
  return vis.r;
}


// Apply fn to the propositoin p.
template<typename F, typename R = typename std::result_of<F(Integer_value const&)>::type>
inline R
apply(Value const& v, F fn)
{
  return dispatch(v, fn);
}


#endif
