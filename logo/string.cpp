
#include "string.hpp"

#include <iostream>
#include <iterator>

using Iter = std::istreambuf_iterator<char>;


// Rad the contents of the given input stream
// into the string buffer.
Stringbuf::Stringbuf(std::istream& is)
  : buf_(Iter(is), Iter())
{ }
