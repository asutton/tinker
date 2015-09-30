
#include "parser.hpp"
#include "ast.hpp"


// Parse a proposition.
//
//    prop -> disjunction
Prop const*
Parser::proposition()
{
  return disjunction();
}


// Parse a disjunction.
//
//    disjunction -> disjunction 'or' conjunction
//                 | conjunction
Prop const*
Parser::disjunction()
{
  Prop const* e1 = conjunction();
  while (true) {
    if (match_if(or_tok)) {
      Prop const* e2 = conjunction();
      e1 = on_disjunction(e1, e2);
    } else {
      break;
    }
  }
  return e1;
}


// Parse a conjunction.
//
//    conjunction -> conjunction 'and' primary
//                 | primary
Prop const*
Parser::conjunction()
{
  Prop const* e1 = primary();
  while (true) {
    if (match_if(and_tok)) {
      Prop const* e2 = primary();
      e1 = on_conjunction(e1, e2);
    } else {
      break;
    }
  }
  return e1;
}


// Parse a primary proposition.
//
//    primary -> identifier
//             | '(' proposition ')'
Prop const*
Parser::primary()
{
  if (Token tok = match_if(identifier_tok))
    return on_identifier(tok);

  if (match_if(lparen_tok)) {
    Prop const* e = proposition();
    match(rparen_tok);
    return e;
  }

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


Prop const*
Parser::on_identifier(Token tok)
{
  return new Atom(tok.symbol());
}


Prop const*
Parser::on_conjunction(Prop const* e1, Prop const* e2)
{
  return new And(e1, e2);
}


Prop const*
Parser::on_disjunction(Prop const* e1, Prop const* e2)
{
  return new Or(e1, e2);
}

