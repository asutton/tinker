

#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "hash.hpp"
#include "simplify.hpp"

#include <iostream>


using namespace std;


// Create a keyword in the symbol table.
inline void
install_keyword(Symbol_table& syms, char const* s, int k)
{
  syms.put<Symbol>(s, k);
}


int 
main()
{
  // Create the symbol table and install all of the 
  // default tokens.
  Symbol_table syms;
  install_keyword(syms, "and", and_tok);
  install_keyword(syms, "or",  or_tok);
  
  // Create the initial streambuf. This reads from cin.
  Char_stream cs = cin;

  // Create the token stream over. This will be populated
  // by the lexer.
  Token_stream ts;

  // Build and run the lexer.
  Lexer lex(syms, cs);
  lex.lex(ts);

  // Parse.
  Parser parse(ts);
  Prop const* p1 = parse.proposition();
  std::cout << "input:  " << p1 << '\n';
  
  Prop const* p2 = simplify(p1);
  std::cout << "simple: " << p2 << '\n';

  std::cout << hash_value(p1) << '\n';
  std::cout << hash_value(p2) << '\n';
}
