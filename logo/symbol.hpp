
#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "string.hpp"

#include <unordered_map>
#include <typeinfo>


// -------------------------------------------------------------------------- //
//                            Symbols

// The base class of all symbols of a language. By
// itself, this class is capable of representing
// symbols that have no other attributes such as
// punctuators and operators.
class Symbol
{
  friend struct Symbol_table;

public:
  Symbol(int);

  virtual ~Symbol() { }

  String const& spelling() const;
  int           token() const;

private:
  String const* str_; // The textual representation
  int           tok_; // The associated token kind
};


inline
Symbol::Symbol(int k)
  : str_(nullptr), tok_(k)
{ }


// Returns the spelling of the symbol.
inline String const& 
Symbol::spelling() const
{
  return *str_;
}


// Returns the kind of token classfication of 
// the symbol.
inline int 
Symbol::token() const 
{ 
  return tok_; 
}


// An identifier symbol.
//
// TODO: Since identifiers are unique interpreted
// symbols, we don't need any information here.
struct Identifier_sym : Symbol
{
  using Symbol::Symbol;
};


// -------------------------------------------------------------------------- //
//                           Symbol table


// The symbol table maintains a mapping of
// unique string values to their corresponding
// symbols.
//
// TODO: Revisit the design of this table to
// allow for efficient insertion and lookup
// of string views.
struct Symbol_table : std::unordered_map<std::string, Symbol*>
{
  ~Symbol_table();

  template<typename T, typename... Args>
  Symbol* put(String const&, Args&&...);
  
  template<typename T, typename... Args>
  Symbol* put(char const*, char const*, Args&&...);

  Symbol const* get(String const&) const;
  Symbol const* get(char const*) const;
};


// Delete allocated resources.
inline
Symbol_table::~Symbol_table()
{
  for (auto const& x : *this)
    delete x.second;
}


// Insert a new symbol into the table. The spelling
// of the symbol is given by the string s and the
// attributes are given in args.
//
// Note that the type of the symbol must be given
// explicitly, and it must derive from the Symbol
// class.
//
// If the symbol already exists, no insertion is
// performed. If new symbol is of a different kind
// (e.g., redefining an integer as an identifier),
// a runtime error is thrown.
//
// TODO: It might also be helpful to verify the
// attributes of re-inserted symbols. That's a bit
// harder.
template<typename T, typename... Args>
Symbol*
Symbol_table::put(String const& s, Args&&... args)
{
  auto x = emplace(s, nullptr);
  auto iter = x.first;
  if (x.second) {
    // Insertion succeeded, so create a new symbol
    // and bind its string representation.
    iter->second = new T(std::forward<Args>(args)...);
    iter->second->str_ = &iter->first;
  } else {
    // Insertion did not succeed. Check that we have
    // not redefined the symbol kind.
    if(typeid(T) != typeid(*iter->second))
      throw std::runtime_error("lexical symbol redefinition");
  }
  return iter->second;

}


// Insert a symbol with the spelling [first, last) and
// the properties in args...
template<typename T, typename... Args>
inline Symbol*
Symbol_table::put(char const* first, char const* last, Args&&... args)
{
  return this->template put<T>(String(first, last), std::forward<Args>(args)...);
}


// Returns the symbol with the given spelling or
// nullptr if no such symbol exists.
inline Symbol const*
Symbol_table::get(String const& s) const
{
  auto iter = find(s);
  if (iter != end())
    return iter->second;
  else
    return nullptr;
}


// Returns the symbol with the given spelling or
// nullptr if no such symbol exists.
inline Symbol const*
Symbol_table::get(char const* s) const
{
  auto iter = find(s);
  if (iter != end())
    return iter->second;
  else
    return nullptr;
}


#endif

