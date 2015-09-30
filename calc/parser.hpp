
#ifndef PARSER_HPP
#define PARSER_HPP

#include "string.hpp"
#include "token.hpp"


struct Expr;


class Parser
{
public:
  Parser(Token_stream&);

  // Parsers
  Expr const* expr();
  Expr const* factor();
  Expr const* term();

private:
  // Actions
  Expr const* on_int(Token);
  Expr const* on_add(Expr const*, Expr const*);
  Expr const* on_sub(Expr const*, Expr const*);
  Expr const* on_mul(Expr const*, Expr const*);
  Expr const* on_div(Expr const*, Expr const*);
  Expr const* on_mod(Expr const*, Expr const*);

  // Parsing support
  Token lookahead() const;
  Token match(Token_kind);
  Token match_if(Token_kind);
  Token accept();

private:
  Token_stream& ts_;
};


inline
Parser::Parser(Token_stream& t)
  : ts_(t)
{ }


// Returns the first token of lookahead.
inline Token
Parser::lookahead() const
{
  return ts_.peek();
}


#endif
