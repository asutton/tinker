

#include "lexer.hpp"
#include "parser.hpp"

#include <iostream>


using namespace std;

int 
main()
{
  // Create the symbol table and install all of the 
  // default tokens.
  Symbol_table syms;
  syms.put<Symbol>("(");
  syms.put<Symbol>(")");
  syms.put<Symbol>("+");
  syms.put<Symbol>("-");
  syms.put<Symbol>("*");
  syms.put<Symbol>("/");
  syms.put<Symbol>("%");
  
  // Create the initial streambuf. This reads from cin.
  Char_stream cs = cin;

  // Create the token stream over. This will be populated
  // by the lexer.
  Token_stream ts;

  // Build and run the lexer.
  Lexer lex(syms, cs);
  lex.lex(ts);

  Parser parse(ts);
  parse.expr();
}