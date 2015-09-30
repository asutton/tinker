
#ifndef LEXER_HPP
#define LEXER_HPP

#include "string.hpp"
#include "token.hpp"

#include <cassert>
#include <cctype>


// The lexer is responsible for the transformation
// of a character stream into a list of tokens.
// FIXME: Maintain source code locations.
class Lexer
{
public:
  using State_flags = unsigned;
  static constexpr State_flags eof_flag   = 0x01;
  static constexpr State_flags error_flag = 0x02;

  Lexer(Symbol_table&, Char_stream&);

  // Lexer state
  bool done() const;
  bool failed() const;

  // Lexing
  bool lex(Token_stream&);
  bool scan(Token_stream&);

  // Scanning
  Token scan();
  Token integer();
  Token lparen();
  Token rparen();
  Token plus();
  Token minus();
  Token star();
  Token slash();
  Token percent();
  Token eof();
  Token error();

private:
  // Semantic actions
  Token on_token(Token_kind);
  Token on_integer(char const*, char const*);

  // Lexing support
  char peek() const { return cs_.peek(); }
  char get() const  { return cs_.get(); }

  Token token(Token_kind); 

  void space();
  void digit();

  State_flags   state_; // The lexer's state
  Symbol_table& syms_;  // The symbol table
  Char_stream&  cs_;    // The character stream
};


inline
Lexer::Lexer(Symbol_table& s, Char_stream& cs)
  : syms_(s), cs_(cs)
{ }


// Returns true if the lexer has finsihed processing
// the character stream.
inline bool
Lexer::done() const
{
  return state_ & eof_flag;
}


// Returns true if the lexer encountered errors while
// processing the character stream.
inline bool
Lexer::failed() const
{
  return state_ & error_flag;
}


// Lexically analyze the underlying character stream,
// and produce a sequence of tokens. Returns true if
// scanning succeeded.
inline bool
Lexer::lex(Token_stream& ts)
{
  while (!done())
    scan(ts);
  return !failed();
}


// Put the next token into the token stream. Returns
// true if scanning succeeded.
inline bool
Lexer::scan(Token_stream& ts)
{
  if (Token tok = scan()) {
    ts.put(tok);
    return true;
  }
  return false;
}


inline Token
Lexer::lparen()
{
  return on_token(lparen_tok);
}


inline Token
Lexer::rparen()
{
  return on_token(rparen_tok);
}


inline Token
Lexer::plus()
{
  return on_token(plus_tok);
}


inline Token
Lexer::minus()
{
  return on_token(minus_tok);
}


inline Token
Lexer::star()
{
  return on_token(star_tok);
}


inline Token
Lexer::slash()
{
  return on_token(slash_tok);
}


inline Token
Lexer::percent()
{
  return on_token(percent_tok);
}


// integer ::= digit+
inline Token
Lexer::integer()
{
  char const* first = cs_.position();
  digit();
  while (is_decimal_digit(peek()))
    digit();
  char const* last = cs_.position();
  return on_integer(first, last);
}


// Set the eof flag and return an invalid token.
inline Token
Lexer::eof()
{
  state_ |= eof_flag;
  return Token();
}


// digit ::= [0-9]
inline void
Lexer::digit()
{
  assert(is_decimal_digit(peek()));
  get();
}


#endif
