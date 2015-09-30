
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
    case '+': return plus();
    case '-': return minus();
    case '*': return star();
    case '/': return slash();
    case '%': return percent();
    
    case '0':
    case '1':
    case '2':
    case '3':
    case '4': 
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return integer();

    default:
      return error();
  }
}


// Get the matched character and return a token.
// Note that the symbol for this token must have
// been previously installed in the symbol table.
//
// TODO: Support multi-character tokens.
inline Token
Lexer::on_token()
{
  Symbol const* sym = syms_.get(build_.take());
  return Token(sym->token(), sym);
}


// Return a new integer token.
inline Token
Lexer::on_integer()
{
  String str = build_.take();
  int n = string_to_int<int>(str, 10);
  Symbol* sym = syms_.put<Integer_sym>(str, integer_tok, n);
  return Token(integer_tok, sym);
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
      ignore();
    else if (is_newline(c))
      ignore();
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
