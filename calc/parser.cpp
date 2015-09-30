
#include "parser.hpp"
#include "ast.hpp"


// Parse an expression.
//
//    expr -> expr ('+' | '-') factor
//          | factor
Expr const*
Parser::expr()
{
  Expr const* e1 = factor();
  while (true) {
    // TODO: Use a switch?
    if (match_if(plus_tok)) {
      Expr const* e2 = factor();
      e1 = on_add(e1, e2);
    } else if (match_if(minus_tok)) {
      Expr const* e2 = factor();
      e1 = on_sub(e1, e2);
    } else {
      break;
    }
  }
  return e1;
}


// Parse a factor.
//
//    factor:
//      factor ('*'' | '/' | '%) term
//      term
Expr const*
Parser::factor()
{
  Expr const* e1 = term();
  while (true) {
    // TODO: Use a switch?
    if (match_if(star_tok)) {
      Expr const* e2 = term();
      e1 = on_mul(e1, e2);
    } else if (match_if(slash_tok)) {
      Expr const* e2 = term();
      e1 = on_div(e1, e2);
    } else if (match_if(percent_tok)) {
      Expr const* e2 = term();
      e1 = on_mod(e1, e2);
    } else {
      break;
    }
  }
  return e1;
}


// Parse a term.
//
//    term:
//      integer
//      '(' expr ')'
Expr const*
Parser::term()
{
  if (match_if(lparen_tok)) {
    Expr const* e = expr();
    match(rparen_tok);
    return e;
  }

  if (Token tok = match_if(integer_tok))
    return on_int(tok);

  // FIXME: Is this definitely an error?
  //
  // FIXME: Diagnostics are awful.
  throw std::runtime_error("syntax error");
}


Token
Parser::match(Token_kind k)
{
  if (ts_.peek().kind() == k)
    return ts_.get();
  
  // FIXME: This is bad.
  throw std::runtime_error("syntax error");
}


// If the current token matches k, return the token
// and advance the stream. Otherwise, return an
// invalid token.
//
// Note that invalid tokens evaluate to false.
Token
Parser::match_if(Token_kind k)
{
  if (ts_.peek().kind() == k)
    return ts_.get();
  else
    return Token();
}


// Returns the current token and advances the
// underlying token stream.
Token
Parser::accept()
{
  return ts_.get();
}


Expr const*
Parser::on_int(Token tok)
{
  // TODO: Get the integer value from the token's 
  // symbol. Need a dynamic cast to do that.
  return new Int_expr(0);
}


Expr const*
Parser::on_add(Expr const* e1, Expr const* e2)
{
  return new Binary_expr(add_op, e1, e2);
}


Expr const*
Parser::on_sub(Expr const* e1, Expr const* e2)
{
  return new Binary_expr(sub_op, e1, e2);
}


Expr const*
Parser::on_mul(Expr const* e1, Expr const* e2)
{
  return new Binary_expr(mul_op, e1, e2);
}


Expr const*
Parser::on_div(Expr const* e1, Expr const* e2)
{
  return new Binary_expr(div_op, e1, e2);
}


Expr const*
Parser::on_mod(Expr const* e1, Expr const* e2)
{
  return new Binary_expr(mod_op, e1, e2);
}

