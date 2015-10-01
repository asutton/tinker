
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
  Token on_token();
  Token on_integer();

  // Lexing support
  char peek() const;
  char get();
  void ignore();

  // Token constructors
  Token symbol(); 

  // Lexers
  void space();
  void digit();

  String_builder build_; // Cache characters during scan.
  State_flags    state_; // The lexer's state
  Symbol_table&  syms_;  // The symbol table
  Char_stream&   cs_;    // The character stream
};


inline
Lexer::Lexer(Symbol_table& s, Char_stream& cs)
  : build_(), state_(0), syms_(s), cs_(cs)
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


// Consume all 1-character symbols and create call
// the corresponding semantic action.
inline Token
Lexer::symbol()
{
  get();
  return on_token();
}


inline Token
Lexer::lparen()
{
  return symbol();
}


inline Token
Lexer::rparen()
{
  return symbol();
}


inline Token
Lexer::plus()
{
  return symbol();
}


inline Token
Lexer::minus()
{
  return symbol();
}


inline Token
Lexer::star()
{
  return symbol();
}


inline Token
Lexer::slash()
{
  return symbol();
}


inline Token
Lexer::percent()
{
  return symbol();
}


// integer ::= digit+
inline Token
Lexer::integer()
{
  digit();
  while (is_decimal_digit(peek()))
    digit();
  return on_integer();
}


// digit ::= [0-9]
inline void
Lexer::digit()
{
  assert(is_decimal_digit(peek()));
  get();
}


// Set the eof flag and return an invalid token.
inline Token
Lexer::eof()
{
  state_ |= eof_flag;
  return Token();
}


inline char 
Lexer::peek() const 
{ 
  return cs_.peek(); 
}


inline char 
Lexer::get()
{ 
  char c = cs_.get(); 
  build_.put(c);
  return c;
}


inline void
Lexer::ignore()
{ 
  cs_.get(); 
}



#endif
