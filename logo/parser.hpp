
#ifndef PARSER_HPP
#define PARSER_HPP

#include "string.hpp"
#include "token.hpp"


struct Prop;


class Parser
{
public:
  Parser(Token_stream&);

  // Parsers
  Prop const* proposition();
  Prop const* disjunction();
  Prop const* conjunction();
  Prop const* primary();

private:
  // Actions
  Prop const* on_identifier(Token);
  Prop const* on_conjunction(Prop const*, Prop const*);
  Prop const* on_disjunction(Prop const*, Prop const*);

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
