#ifndef ryk_iterable_algorithms
#define ryk_iterable_algorithms

#include <algorithm>
#include <numeric>
#include <set>
#include <vector>

#include "traits.hpp"
#include "predicates.hpp"

namespace ryk {

//
// combine - performs a function on each pair of two iterables
// another name might be for_each_pair
//
template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryFn>
inline constexpr
std::enable_if_t<is_iterator_v<InputIterator1> && is_iterator_v<InputIterator2>
                 && is_iterator_v<OutputIterator> 
                 && is_binary_function_v<BinaryFn, deref<InputIterator1>, deref<InputIterator2>>,
                 OutputIterator>
combine(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
        InputIterator2 last2, OutputIterator out, BinaryFn f)
{
  while (first1 != last1 && first2 != last2) *out++ = f(*first1++, *first2++);
  return out;
}
template<class Iterable1, class Iterable2, class OutputIterator, class BinaryFn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2> 
                 && is_iterator_v<OutputIterator> 
                 && is_binary_function_v<BinaryFn, subtype<Iterable1>, subtype<Iterable2>>, 
                 OutputIterator>
combine(const Iterable1& c1, const Iterable2& c2, OutputIterator out, BinaryFn f)
{
  return combine(c1.begin(), c1.end(), c2.begin(), c2.end(), out, f);
}
template<class Iterable1, class Iterable2, class OutputIterable, class BinaryFn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2> 
                 && is_iterable_v<OutputIterable> 
                 && is_binary_function_v<BinaryFn, subtype<Iterable1>, subtype<Iterable2>>, 
                 OutputIterable&>
combine(const Iterable1& c1, const Iterable2& c2, OutputIterable& out, BinaryFn f)
{
  combine(c1, c2, out.begin(), f);
  return out;
}
template<template<class> class Iterable1, class Iterable2, class BinaryFn, class Ts..> 
inline constexpr
std::enable_if_t<is_iterable_v<Iterable1<Ts...>> && is_iterable_v<Iterable2> 
                 && is_binary_fn_v<BinaryFn>, 
                 Iterable1<std::result_of_t<BinaryFn(subtype<Iterable1>, subtype<Iterable2>)>>>
combine(const Iterable1& c1, const Iterable2& c2, BinaryFn f)
{
  auto r = construct_with_size(std::min(c1.size(), c2.size()), 
             Iterable1<std::result_of_t<BinaryFn(subtype<Iterable1>, subtype<Iterable2>)>>);
  return combine(c1, c2, r, f);
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
