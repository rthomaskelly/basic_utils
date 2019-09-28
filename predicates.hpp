#ifndef ryk_predicates
#define ryk_predicates

#include "boxed_types.hpp"

namespace ryk {

//
// is_even(T) and is_odd(T) with Lisp-named counterparts for arithmetic types
//
template<class T> inline constexpr
std::enable_if_t<is_arithmetic_v<T>, bool>
is_even(const T& t)
{
  return t & 1;
}
template<class T> inline constexpr
std::enable_if_t<is_arithmetic_v<T>, bool>
evenp(const T& t)
{
  return is_even(t);
}
template<class T> inline constexpr
std::enable_if_t<is_arithmetic_v<T>, bool>
is_odd(const T& t)
{
  return !is_even(t);
}
template<class T> inline constexpr
std::enable_if_t<is_arithmetic_v<T>, bool>
oddp(const T& t)
{
  return is_odd(t);
}

} // namespace ryk

#endif
