
#ifndef AST_HPP
#define AST_HPP


struct Expr
{
  virtual ~Expr() { }
};


struct Int_expr : Expr
{
  Int_expr(int n)
    : val_(n)
  { }

  int val_;
};


enum Binary_op 
{
  add_op,
  sub_op,
  mul_op,
  div_op,
  mod_op,
};


struct Binary_expr : Expr
{
  Binary_expr(int op, Expr const* e1, Expr const* e2)
    : first(e1), second(e2)
  { }

  Binary_op   op;
  Expr const* first;
  Expr const* second;
};


#endif
