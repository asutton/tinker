

#include "lexer.hpp"
#include "parser.hpp"

#include <iostream>


using namespace std;

int 
main(int argc, char* argv[])
{
  // FIXME: This is dumb.
  bool dont_parse = false;
  if (argc > 1 && argv[1][0] == 'l')
    dont_parse = true;    

  // Create the symbol table and install all of the 
  // default tokens.
  Symbol_table syms;
  syms.put<Symbol>("(", lparen_tok);
  syms.put<Symbol>(")", rparen_tok);
  syms.put<Symbol>("+", plus_tok);
  syms.put<Symbol>("-", minus_tok);
  syms.put<Symbol>("*", star_tok);
  syms.put<Symbol>("/", slash_tok);
  syms.put<Symbol>("%", percent_tok);
  
  // Create the initial streambuf. This reads from cin.
  Char_stream cs = cin;

  // Create the token stream over. This will be populated
  // by the lexer.
  Token_stream ts;

  // Build and run the lexer.
  Lexer lex(syms, cs);
  lex.lex(ts);
  if (dont_parse)
    return 0;

  Parser parse(ts);
  parse.expr();
}