
#include "lexer.hpp"


#include <iostream>


// Returns the next token in the character stream.
// If no next token can be identified, an error
// is emitted and we return the error token.
Token
Lexer::scan()
{
  // Consume any white space here.
  space();

  switch (peek()) {
    case 0: return eof();

    case '(': return lparen();
    case ')': return rparen();

    case '-':
      if (peek(1) == '>')
        return arrow();
      else
        return error();

    default:
      if (Token tok = identifier())
        return tok;
      else
        return error();
  }
}


// Get the matched sequence of characters and return 
// a token.
inline Token
Lexer::on_symbol(int n, Token_kind k)
{
  // Get the string bounds and consume the characters.
  //
  // TODO: Define get(n) for the character stream.
  char const* first = cs_.position();
  char const* last = first + n;
  ignore(n);

  // Get the symbol and return the token.
  Symbol const* sym = syms_.get(String(first, last));
  return Token(k, sym);
}


// Return a new identifier token.
inline Token
Lexer::on_identifier(char const* first, char const* last)
{
  // Lookup the symbol first.
  String str(first, last);  
  if (Symbol const* sym = syms_.get(str))
    return Token(sym->token(), sym);

  // Create a new symbol.  
  Symbol* sym = syms_.put<Identifier_sym>(str, identifier_tok);
  return Token(identifier_tok, sym);
}


// Consume horizontal and vertical whitespace.
//
// TODO: Manage the input location while lexing.
void
Lexer::space()
{
  while (true) {
    char c = peek();
    if (is_space(c))
      get();
    else if (is_newline(c))
      get();
    else
      break;
  }
}


// Set the error flag and return an invalid token.
// This also consumes the current character so that
// we can continue lexing.
inline Token
Lexer::error()
{
  // TODO: Improve diagnostics.
  std::cerr << "error: invalid character '" << peek() << "'\n";

  state_ |= error_flag;
  get();
  return Token();
}
