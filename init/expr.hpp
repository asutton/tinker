
#ifndef EXPR_HPP
#define EXPR_HPP


#include "prelude.hpp"
#include "value.hpp"


struct Expr
{
  virtual ~Expr() { }
};


// An expression containing a literal value.
struct Constant_expr : Expr
{
  Constant_expr(Value const* v)
    : value_(v)
  { }

  Value const* value() const { return value_; }

  Value const* value_;
};


// An expression of type tuple.
//
//    {e1, ..., en}
struct Tuple_expr : Expr
{
  Tuple_expr(Expr_seq const& e)
    : first(e)
  { }

  Expr_seq const& elements() const { return first; }

  Expr_seq first;
};



// Copy initialization.
//
//    var x : int = e;
//
// This invokes a constructor in T that takes
// `e` as an argument.
struct Copy_init : Expr
{
  Expr const* first;
};


// Direct initialization:
//
//    var x : T(e1, ..., en);
//
// This invokes the constructor in T with the given
// arguments.
struct Direct_init : Expr
{
  Expr_seq first;
};


// Aggregate initialization:
//
//    var x : T { e1, ..., en }
//
// Aggregate initialization directly initializes an
// object from sequence expressions enclosed in braces.
//
// TODO: Does this invoke a constructor?
struct Aggregate_init : Expr
{
  Expr_seq first;
};


#endif
