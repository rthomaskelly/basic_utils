#ifndef ryk_iterable_algorithms
#define ryk_iterable_algorithms

#include <algorithm>
#include <numeric>
#include <set>
#include <vector>

#include "traits.hpp"
#include "predicates.hpp"

namespace ryk {

// morph requires a 'push' function to be implemented on the iterable type
template<template<class> class Iterable, class T, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable<T>>, Iterable<std::result_of_t<Fn(T)>>>
morph(const Iterable<T>& c, Fn f)
{
  Iterable<std::result_of_t<Fn(T)>> r; 
  for (auto& t : c) push(r, f(t));
  return r; 
}

//
// tie - performs a function on each pair of two iterables
// TODO: finish, currently does NOT work
template<class Iterable0, class Iterable1, class R, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable0> && is_iterable_v<Iterable1>, R>
tie(const Iterable0& c0, const Iterable1& c1, Fn f)
{
  auto i0 = c0.begin();
  auto i1 = c1.begin();
  for(; i0 < c0.end() && i1 < c1.end(); ++i0, ++i1) f(*i0, *i1);
  return c0;
}

//
// circle_index implements python-like negative indices
//
template<class Sizeable> inline constexpr
subtype<Sizeable> circle_index(Sizeable& c, int64_t index)
{
  // the following is equivalent to:
  //   if (index < 0) index + c.size();
  //   else index;
  // but the bit-wise operation of multiplying by the sign bit s/b done in a few cycles

  return at(c, index + c.size() * (index >> 63));
}

template<class Iterable, class Pred>
std::enable_if_t<is_iterable<Iterable>::value, std::vector<subtype<Iterable>>>
which(const Iterable& c, Pred p) {
  std::vector<subtype<Iterable>> ret;
  for (auto& each : c) if (p(each)) ret.push_back(each);
  return ret;
}
template<class Iterable, bool(*F)(const subtype<Iterable>&)>
std::enable_if_t<is_iterable<Iterable>::value, std::vector<subtype<Iterable>>>
which(const Iterable& c) {
  std::vector<subtype<Iterable>> ret;
  for (auto& each : c) if (F(each)) ret.push_back(each);
  return ret;
}

//template<class T, template<class> class Iterable1, template<class> class Iterable2> inline constexpr
//std::enable_if_t<is_iterable<Iterable1<T>>::value && is_iterable<Iterable2<uint64_t>>::value, Iterable2<T>>
//slice(const Iterable1<T>& c, const Iterable2<uint64_t> indices) {
//  Iterable2<T> ret;
//  // for (uint64_t n : indices) ret[
//  return ret;
//}

template<class Indexable, class IndicesIterable> inline constexpr
std::enable_if_t<is_indexable_v<Indexable> && is_iterable_v<IndicesIterable>
                 && std::is_arithmetic_v<subtype<IndicesIterable>>, Indexable>
slice(const Indexable& c, const IndicesIterable& indices)
{
  Indexable r(indices.size());
  std::size_t j = 0;
  for (auto i : indices) r[j++] = c[i]; 
  return r;
}


template<class T, template<class, class alloc = std::allocator<T>> class Iterable, class Pred>
std::enable_if_t<is_iterable<Iterable<T>>::value, std::vector<T>>
select(const Iterable<T>& c, Pred p) {
  std::vector<T> ret;
  for (auto& each : c) if (p(each)) ret.push_back(each);
  return ret;
}

} // namespace
