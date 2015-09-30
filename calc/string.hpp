
#ifndef STRING_HPP
#define STRING_HPP

#include <cstring>
#include <algorithm>
#include <iosfwd>
#include <string>
#include <stdexcept>


// -------------------------------------------------------------------------- //
//                              Strings

// The string type.
using String = std::string;


// Returns true if c is the horizontal whitespace.
// Note that vertical tabs and carriage returns
// are considered horizontal white space.
inline bool
is_space(char c)
{
  switch (c) {
    case ' ':
    case '\t':
    case '\r':
    case '\v':
      return true;
    default:
      return false;
  }
}


// Returns true if c is a newline character.
inline bool
is_newline(char c)
{
  return c == '\n';
}


// Returns true if c in the class [01].
inline bool
is_binary_digit(char c)
{
  return (c - '0' < 2);
}


// Returns true if c is in the class [0-9].
inline bool
is_decimal_digit(char c)
{
  return std::isdigit(c);
}


// If is a digit in base d, return n. Otherwise,
// throw a runtime error.
inline int
if_in_base(int n, int b)
{
  if (n < b)
    return n;
  else
    throw std::runtime_error("invalid digit");
}


// Returns the integer value of a character in base b,
// or -1 if the character is not a digit in that base.
inline int
char_to_int(char c, int b)
{
  if ('0' <= c && c <= '9')
    return if_in_base(c - '0', b);
  if ('a' <= c && c <= 'z')
    return if_in_base(c - 'a' + 10, b);
  if ('A' <= c && c <= 'Z')
    return if_in_base(c - 'A' + 10, b);
  return if_in_base(c, 0);
}


// Returns the integer value of the string in [first, last),
// which contains an integer representation in base b. If
// [first, last) contains any characters that are not digits
// in base b, this throws a runtime error.
//
// Note that T must be an integer type and is given as an
// explicit template argument.
template<typename T, typename I>
inline T
string_to_int(I first, I last, int b)
{
  T n = 0;
  while (first != last) {
    n += n * b + char_to_int(*first, b);
    ++first;
  }
  return n;
}


// Returns the integer value of a string containing an
// integer representation in base b. If s contains any
// characters that are not digits in base b, this throws
// a runtime error.
template<typename T>
inline T
string_to_int(String const& s, int b)
{
  return string_to_int<T>(s.begin(), s.end(), b);
}


// -------------------------------------------------------------------------- //
//                            String buffer


// The string buffer class provides implements a simple 
// string-based buffer for a stream. The string must not 
// have null characters.
class Stringbuf
{
public:
  Stringbuf(String const&);
  Stringbuf(std::istream& is);

  char const* begin() const;
  char const* end() const;

private:
  String buf_;
};



// Initialize the sting buffer from a pre-existing
// string. Note that this copies the string.
inline
Stringbuf::Stringbuf(String const& s)
  : buf_(s)
{ }


// Returns an iterator to the beginning of the string
// buffer.
inline char const* 
Stringbuf::begin() const
{ 
  return buf_.c_str(); 
}


// Returns an iterator past the end of the string buffer.
inline char const* 
Stringbuf::end() const
{ 
  return begin() + buf_.size(); 
}


// -------------------------------------------------------------------------- //
//                          String builder


// The string builder facilitates the caching of characters
// needed to form a string during lexical analysis.
//
// TODO: Allow strings greater than 32 bytes in length.
// Basically, this entails implementing the small-string 
// optimization.
class String_builder
{
  static constexpr int init_size = 32;
public:
  String_builder();

  String str() const;
  String take();

  void put(char c);
  void put(char const*);
  void put(char const*, int n);
  void put(char const*, char const*);

  void clear();
  
private:
  char buf_[init_size];
  int len_;
};


inline
String_builder::String_builder()
  : len_(0)
{ 
  std::fill(buf_, buf_ + init_size, 0);
}


// FIXME: This should return a string view, but until
// we can efficiently hash-compare a string view against
// a string, it won't matter.
inline String
String_builder::str() const
{
  return String(buf_, buf_ + len_);
}


// Return the string in the builder and then reset it.
inline String
String_builder::take()
{
  String s = str();
  clear();
  return s;
}


inline void
String_builder::put(char c)
{
  if (len_ == init_size)
    throw std::runtime_error("out of string memory");
  buf_[len_++] = c;
}


inline void
String_builder::put(char const* s)
{
  put(s, std::strlen(s));
}


inline void
String_builder::put(char const* s, int n)
{
  if (init_size <= len_ + n)
    throw std::runtime_error("out of string memory");
  std::copy_n(s, n, buf_ + len_);
  len_ += n;
}


inline void
String_builder::put(char const* first, char const* last)
{
  put(first, last - first);
}


// Reset the string builder so that it's content
// is empty. Also, zero-fill the buffer so we
// don't have to think about null-terminating
// strings.
inline void
String_builder::clear()
{
  len_ = 0;
  std::fill(buf_, buf_ + init_size, 0);
}


// -------------------------------------------------------------------------- //
//                          Character stream

// The character stream class provides a stream abstraction
// on top of a string buffer.
//
// The stream buffer allows the position of a character to
// be returned, which allows a lexer to save the bounds of
// a symbol. An alternative would be to have the lexer
// buffer accepted characters. Note that truly buffered
// input streams would not be able to return an iterator
// to the current character.
//
// TODO: Allow the stream buffer to be shared by multiple
// streams?
class Char_stream
{
public:
  using Position = char const*;

  Char_stream(String const&);
  Char_stream(std::istream&);

  bool eof() const;
  
  char peek() const;
  char get();

  Position position() const;

private:
  Stringbuf buf_; // The shared buffer.
  Position  pos_; // The current position.
};


inline
Char_stream::Char_stream(String const& s)
  : buf_(s), pos_(buf_.begin())
{ }


inline
Char_stream::Char_stream(std::istream& is)
  : buf_(is), pos_(buf_.begin())
{ }


// Returns true if the stream is at the end
// of the file.
inline bool
Char_stream::eof() const
{
  return pos_ == buf_.end();
}


// Returns the current character. If at the end
inline char
Char_stream::peek() const
{
  if (eof())
    return 0;
  else
    return *pos_;
}


// Returns the current character and advances the
// stream.
inline char
Char_stream::get()
{
  if (eof())
    return 0;
  else
    return *pos_++;
}


// Returns a pointer to the character at the current
// position. 
inline Char_stream::Position
Char_stream::position() const 
{ 
  return pos_; 
}


#endif
