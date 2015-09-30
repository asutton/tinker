
#ifndef CAST_HPP
#define CAST_HPP

#include <cassert>


template<typename U, typename T>
inline U*
as(T* t)
{
  return dynamic_cast<U*>(t);
}


template<typename U, typename T>
inline U const*
as(T const* t)
{
  return dynamic_cast<U const*>(t);
}


template<typename U, typename T>
inline bool
is(T const* t)
{
  return dynamic_cast<U const*>(t);
}


template<typename U, typename T>
inline U*
cast(T* t)
{
  assert(is<U>(t));
  return static_cast<U*>(t);
}


template<typename U, typename T>
inline U const*
cast(T const* t)
{
  assert(is<U>(t));
  return static_cast<U const*>(t);
}


#endif
