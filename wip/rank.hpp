#ifndef ryk_rank
#define ryk_rank

#include "traits.hpp"

//
// TODO: review our implementations with what is available in STL
// currently I don't believe much of this works
//



namespace ryk {
// 
// rank definitions for scalars, iterables and boxed_types
// rank is used here as in the a n-rank tensor
// sometimes called 'shape' in tensor libraries
//

// template<class T>
// struct rank 
//  : public std::integral_constant<std::enable_if_t<std::is_scalar<T>::value, std::size_t>, 0> {};
//  
// template<class T>
// struct rank<T[]> : public std::integral_constant<std::size_t, rank<T>::value + 1> {};
//  
// template<class T, std::size_t N>
// struct rank<T[N]> : public std::integral_constant<std::size_t, rank<T>::value + 1> {};
// 
// template<template<class> class Iterable, class... Ts>
// struct rank<Iterable<Ts...> 
//  : public std::integral_constant<std::enable_if_t<is_iterable_v<Iterable<Ts...>>, std::size_t> 
//                                  rank<subtype<Iterable<Ts...>>>::value + 1> {};
// 
// template<class T> inline constexpr bool rank_v = rank<T>::value;

template<class T> inline constexpr 
bool has_rank = std::is_scalar<T>::value || is_iterable_v<T> || is_boxed_type_v<T>;
template<class T, class V> inline constexpr
bool have_rank = has_rank<T> && has_rank<V>;

template<class T> inline constexpr
std::enable_if_t<std::is_scalar<T>::value, std::size_t>
rank()
{
  return 0;
}
template<class T> inline constexpr
std::enable_if_t<is_iterable_v<T>, std::size_t>
rank()
{
  return 1 + rank<subtype<T>>();
}
template<class T> inline constexpr
std::enable_if_t<is_boxed_type_v<T>, std::size_t>
rank()
{
  return rank<typename T::boxedtype>();
}
template<class T> inline constexpr
std::enable_if_t<has_rank<T>, std::size_t>
rank(T) 
{ 
  return rank<T>();
}

template<class T, class V,
enable_if_p<rank<T>() == 1 && rank<V>() == 0>...> inline
auto operator+(const T& ts, const V v)
{
  return transform(ts, [=v](const subtype<T>& t){ return t + v; });
}
template<class T, class V,
enable_if_p<rank<T>() == 0 && rank<V>() == 1>...> inline
auto operator+(const T t, const V& vs)
{
  return vs + t;
}

template<class T, class V,
enable_if_p<rank<T>() == 1 && rank<V>() == 0>...> inline
auto operator-(const T& ts, const V v)
{
  return transform(ts, [=v](const subtype<T>& t){ return t - v; });
}
template<class T, class V,
enable_if_p<rank<T>() == 0 && rank<V>() == 1>...> inline
auto operator-(const T t, const V& vs)
{
  return vs - t;
}

template<class T, class V,
enable_if_p<rank<T>() == 1 && rank<V>() == 0>...> inline
auto operator*(const T& ts, const V v)
{
  return transform(ts, [=v](const subtype<T>& t){ return t * v; });
}
template<class T, class V,
enable_if_p<rank<T>() == 0 && rank<V>() == 1>...> inline
auto operator*(const T t, const V& vs)
{
  return vs * t;
}

template<class T, class V,
enable_if_p<rank<T>() == 1 && rank<V>() == 0>...> inline
auto operator/(const T& ts, const V v)
{
  return transform(ts, [=v](const subtype<T>& t){ return t / v; });
}
template<class T, class V,
enable_if_p<rank<T>() == 0 && rank<V>() == 1>...> inline
auto operator/(const T t, const V& vs)
{
  return vs / t;
}

// template<class T, class V> inline
// void size_check(const T& t, const V& v, const string& function_name)
// {
//   if (t.size() != v.size())
//     throw std::out_of_range("Size mismatch between rank-1 elements in " + function_name + ".");
// }
// template<class T, class V,
// enable_if_p<rank_v<T> == 1 && rank_v<V> == 1>...> inline
// auto operator+(const T& t, const V& v)
// {
//   size_check(t, v, "operator +");
//   return tie(t, v, [](const subtype<T>& tt, const subtype<V>& vv){ return tt + vv; }); 
// }




} // namespace ryk

#endif
