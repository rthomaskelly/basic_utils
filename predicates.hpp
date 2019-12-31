#ifndef ryk_predicates
#define ryk_predicates

#include "boxed_types.hpp"

namespace ryk {

//
// is_even(T) and is_odd(T) with Lisp-named counterparts for arithmetic types
//
template<class T> inline constexpr
std::enable_if_t<std::is_arithmetic_v<T>, bool>
is_even(T t)
{
  return (t & 1) == 0;
}
template<class T> inline constexpr
std::enable_if_t<std::is_arithmetic_v<T>, bool>
evenp(T t)
{
  return is_even(t);
}
template<class T> inline constexpr
std::enable_if_t<std::is_arithmetic_v<T>, bool>
is_odd(T t)
{
  return !is_even(t);
}
template<class T> inline constexpr
std::enable_if_t<std::is_arithmetic_v<T>, bool>
oddp(T t)
{
  return is_odd(t);
}

} // namespace ryk

#endif
