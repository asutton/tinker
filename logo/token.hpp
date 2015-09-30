
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "symbol.hpp"

#include <list>


// -------------------------------------------------------------------------- //
//                            Token kinds

// The different kinds of tokens. Note that we need an
// error token to indicate the invalid tokens returned
// during lexing.
enum Token_kind
{
  error_tok = -1,
  lparen_tok,
  rparen_tok,
  and_tok,
  or_tok,
  arrow_tok,
  identifier_tok,
};


// -------------------------------------------------------------------------- //
//                            Token class

// A classified symbol in the source language.
//
// Note that tokens internally track their kind as
// an integer value. This allows client languages
// to define their own token enumeration wihtout
// having to instantiate a new token class.
//
// TODO: Keep the source code location.
class Token
{
public:
  Token();
  Token(int);
  Token(int, Symbol const*);

  explicit operator bool() const;

  int           kind() const;
  Symbol const* symbol() const;
  String const& spelling() const;

private:
  int           kind_;
  Symbol const* sym_;
};


// Initialize the token to the error token.
inline
Token::Token()
  : Token(-1)
{ }


// Initialize a non-symbol token of kind k.
inline
Token::Token(int k)
  : Token(k, nullptr)
{ }


// Initialize a token of kind k with the given
// symbol table entry.
inline
Token::Token(int k, Symbol const* s)
  : kind_(k), sym_(s)
{ }


// Returns true if the token is valid (i.e.,
// not the error token).
inline
Token::operator bool() const
{
  return kind_ != -1;
}


// Returns the token kind.
inline int 
Token::kind() const
{
  return kind_;
}


// Returns the token's symbol and attributes.
inline Symbol const* 
Token::symbol() const
{
  return sym_;
}


// Returns the spelling of the token.
inline String const&
Token::spelling() const
{
  return sym_->spelling();
}



// -------------------------------------------------------------------------- //
//                            Token buffer


// A token buffer is a finite sequence of tokens.
//
// Note that tokens are maintained in a linked list
// so that modifications to the buffer don't invalidate
// iterators.
//
// TODO: Define appropriate constructors, etc.
struct Tokenbuf : std::list<Token>
{
  using std::list<Token>::list;
};


// -------------------------------------------------------------------------- //
//                            Token stream


// A token stream provides a stream interface to a
// token buffer.
//
// TODO: This is currently modeling a read/write stream.
// We probably need both a read and write stream position,
// although the write position is always at the end.
class Token_stream
{
public:
  using Position = Tokenbuf::iterator;

  Token_stream();

  bool eof() const;

  Token peek() const;
  Token get();
  void put(Token);

  Position position() const; 

private:
  Tokenbuf buf_;
  Position pos_;
};


// Initialize a token stream with an empty
// buffer.
inline
Token_stream::Token_stream()
  : buf_(), pos_(buf_.begin())
{ }


// Returns true if the stream is at the end of the file.
inline bool
Token_stream::eof() const
{
  return pos_ == buf_.end();
}


// Returns the current token.
inline Token
Token_stream::peek() const
{
  if (eof())
    return Token();
  else
    return *pos_;
}


// Returns the current token and advances the stream.
inline Token
Token_stream::get()
{
  if (eof())
    return Token();
  else
    return *pos_++;
}


// Puts the given token at the end of the stream.
inline void
Token_stream::put(Token tok)
{
  buf_.push_back(tok);

  // Make sure that the pos_ isn't pointing past
  // then end after insertion into an empty list.
  if (pos_ == buf_.end())
    pos_ = buf_.begin();
}


// Returns the current position of the stream. This
// effectively an iterator into the buffer.
inline Token_stream::Position
Token_stream::position() const
{
  return pos_;
}


#endif
