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
// STL wrappers and extras
//
template<class T> inline constexpr
std::underlying_type_t<T> underlying_cast(T t)
{
  return static_cast<std::underlying_type_t<T>>(t);
}

//
// find and find_if family
// find_index returns an index instead of an iterator
// index_of is a synonym for find_index
//
template<class Map> inline constexpr
std::enable_if_t<is_map_v<Map>, iterator<Map>>
find(Map& m, const typename Map::key_type& k)
{
  return m.find(k);
}
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && !is_map_v<Iterable>
                 && is_equality_comparable_v<subtype<Iterable>, T>, iterator<Iterable>>
find(Iterable& c, const T& t)
{
  return std::find(c.begin(), c.end(), t);
}
template<class Iterable, class Unary> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_unary_function_v<Unary, subtype<Iterable>>,
                 iterator<Iterable>>
find_if(Iterable& c, Unary f)
{
  return std::find_if(c.begin(), c.end(), f);
}

//
// find_index synonymous with index_of
// returns the index of the iterator instead of the iterator itself
//
template<class Iterator, class T> inline constexpr
size_t find_index(Iterator begin, Iterator end, const T& t)
{
  return std::find(begin, end, t) - begin;
}
template<class Iterator, class T> inline constexpr
size_t index_of(Iterator begin, Iterator end, const T& t)
{
  return find_index(begin, end, t);
}
template<class Iterator, class Fn> inline constexpr
size_t find_if_index(Iterator begin, Iterator end, Fn f)
{
  return std::find_if(begin, end, f) - begin;
}
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && !std::is_same_v<T, iterator<Iterable>>, 
                 distance<Iterable>>
find_index(Iterable& c, const T& t)
{
  return find(c, t) - c.begin();
}
template<class Iterable, class Pred> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
find_if_index(Iterable& c, Pred pred)
{
  return find_if(c, pred) - c.begin();
}
template<class Iterable, class Pred> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
find_index_if(Iterable& c, Pred pred)
{
  return find_if_index(c, pred);
}
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && !std::is_same_v<T, iterator<Iterable>>, 
                 distance<Iterable>>
index_of(Iterable& c, const T& t)
{
  return find_index(c, t);
}
template<class Iterable, class Pred> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
index_of_if(Iterable& c, Pred pred)
{
  return find_if_index(c, pred);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
index_of(const Iterable& c, const iterator<Iterable> i)
{
  return i - c.begin();
}

//
// search - runs find for a given sequence instead of a given value
//
template<class Iterable1, class Iterable2> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>, iterator<Iterable1>>
search(Iterable1& search_in, const Iterable2& search_for)
{
  return std::search(search_in.begin(), search_in.end(), search_for.begin(), search_for.end());
}
template<class Iterable1, class Iterable2, class BinaryFn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>
                 && is_binary_function_v<BinaryFn, subtype<Iterable1>, subtype<Iterable2>>,
                 iterator<Iterable1>>
search(Iterable1& search_in, const Iterable2& search_for, BinaryFn f)
{
  return std::search(search_in.begin(), search_in.end(), search_for.begin(), search_for.end(), f);
}


//
// accumulate and reduce (reduce not yet available in GCC <=9.2)
// please note that its probably proper to have the return type 
// of the second accumulate variation be:
//   std::result_of_t<Fn(T, subtype<Iterable>)>
// but the std::accumulate it calls just returns T, the type of 'init'
//
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> &&
                 !is_binary_function_v<T, subtype<Iterable>, subtype<Iterable>>, T>
accumulate(const Iterable& c, T init = subtype<Iterable>{})
{
  return std::accumulate(c.begin(), c.end(), init);
}
template<class Iterable, class T, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, T>
accumulate(const Iterable& c, T init, Fn f)
{
  return std::accumulate(c.begin(), c.end(), init, f);
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> &&
                 is_binary_function_v<Fn, subtype<Iterable>, subtype<Iterable>>,
                 subtype<Iterable>>
accumulate(const Iterable& c, Fn f)
{
  return std::accumulate(c.begin(), c.end(), subtype<Iterable>{}, f);
}
// template<class Iterable> inline constexpr
// std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
// reduce(const Iterable& c, subtype<Iterable> init = subtype<Iterable>{})
// {
//   return std::reduce(c.begin(), c.end(), init); 
// }
// template<class Iterable, class Fn> inline constexpr
// std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
// reduce(const Iterable& c, subtype<Iterable> init, Fn f)
// {
//   return std::reduce(c.begin(), c.end(), init, Fn f); 
// }
// template<class Iterable, class Fn> inline constexpr
// std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
// reduce(const Iterable& c, Fn f)
// {
//   return reduce(c, subtype<Iterable>{}, f);
// }

//
// sort
//
template<class T> inline constexpr
std::set<T>& sort(const std::set<T>& s)
{
  return s;
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
sort(Iterable& c)
{
  std::sort(c.begin(), c.end());
  return c;
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
sort(Iterable& c, Compare compare)
{
  std::sort(c.begin(), c.end(), compare);
  return c;
}

//
// remove and remove_if
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
remove(Iterable& c, const subtype<Iterable>& t)
{
  return std::remove(c.begin(), c.end(), t);
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
remove_if(Iterable& c, Fn f)
{
  return std::remove_if(c.begin(), c.end(), f);
}

//
// erase_remove and erase_remove_if implements the C++ erase/remove idiom
//
template<class Iterable> inline
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
erase_remove(Iterable& c, const subtype<Iterable>& t)
{
  return c.erase(remove(c, t), c.end());
}
template<class Iterable, class Fn> inline
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
erase_remove_if(Iterable& c, Fn f)
{
  return c.erase(remove_if(c, f), c.end());
}

//
// replace, replace_if, for replace_all look at boost::replace_all
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
replace(Iterable& c, const subtype<Iterable>& old_value, const subtype<Iterable>& new_value)
{
  std::replace(c.begin(), c.end(), old_value, new_value);
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
replace_if(Iterable& c, Fn f, const subtype<Iterable>& new_value)
{
  std::replace_if(c.begin(), c.end(), f, new_value);
}
template<class Iterable, class InIter> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
replace(Iterable& c, const subtype<Iterable>& old_value, 
        InIter new_values_begin, InIter new_values_end)
{
  auto it = find(c, old_value);
  if (it != c.end()) insert(c, it, new_values_begin, new_values_end);
}
template<class Iterable, class InsertIterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<InsertIterable>, iterator<Iterable>>
replace(Iterable& c, const subtype<Iterable>& old_value, const InsertIterable& i)
{
  return replace(c, old_value, i.begin(), i.end());
}

//
// unique and erase_duplicates
// erase_duplicates is the function that will actually erase the duplicates
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
unique(Iterable& c)
{
  return std::unique(c.begin(), c.end());
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
unique(Iterable& c, Compare compare)
{
  return std::unique(c.begin(), c.end(), compare);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
erase_duplicates(Iterable& c)
{
  return c.erase(unique(c), c.end());
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
erase_duplicates(Iterable& c, Compare compare)
{
  return c.erase(unique(c, compare), c.end());
}


//
// reverse
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
reverse(Iterable& c)
{
  return std::reverse(c.begin(), c.end());
}

//
// max_element (max), min_element (min) & minmax_element (minmax)
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
max_element(Iterable& c)
{
  return std::max_element(c.begin(), c.end()); 
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
max_element(Iterable& c, Compare compare)
{
  return std::max_element(c.begin(), c.end(), compare);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
max(Iterable& c)
{
  return max_element(c); 
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
max(Iterable& c, Compare compare)
{
  return max_element(c, compare);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
min_element(Iterable& c)
{
  return std::min_element(c.begin(), c.end()); 
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
min_element(Iterable& c, Compare compare)
{
  return std::min_element(c.begin(), c.end(), compare);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
min(Iterable& c)
{
  return min_element(c); 
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
min(Iterable& c, Compare compare)
{
  return min_element(c, compare);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, std::pair<iterator<Iterable>, iterator<Iterable>>>
minmax_element(Iterable& c)
{
  return std::minmax_element(c.begin(), c.end()); 
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, std::pair<iterator<Iterable>, iterator<Iterable>>>
minmax_element(Iterable& c, Compare compare)
{
  return std::minmax_element(c.begin(), c.end(), compare);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, std::pair<iterator<Iterable>, iterator<Iterable>>>
minmax(Iterable& c)
{
  return minmax_element(c); 
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, std::pair<iterator<Iterable>, iterator<Iterable>>>
minmax(Iterable& c, Compare compare)
{
  return minmax_element(c, compare);
}

//
// all and all_of
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, bool>
all(const Iterable& c)
{
  for (auto& t : c) if (!t) return false;
  return true;
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, bool>
all_of(const Iterable& c, Fn f)
{
  return std::all_of(c.begin(), c.end(), f);
}

//
// any and any_of
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, bool>
any(const Iterable& c)
{
  for (auto& t : c) if (t) return true;
  return false;
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, bool>
any_of(const Iterable& c, Fn f)
{
  return std::any_of(c.begin(), c.end(), f);
}

//
// none and none_of
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, bool>
none(const Iterable& c)
{
  return !any(c);
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, bool>
none_of(const Iterable& c, Fn f)
{
  return std::none_of(c.begin(), c.end(), f);
}

//
// count and count_if
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
count(const Iterable& c, const subtype<Iterable>& t)
{
  return std::count(c.begin(), c.end(), t);
}
template<class Iterable, class UnaryPredicate> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
count_if(const Iterable& c, UnaryPredicate p)
{
  return std::count(c.begin(), c.end(), p);
}

//
// copy and copy_if
//
template<class Iterator, class OutputIterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, OutputIterator>
copy(Iterator it, std::size_t count, OutputIterator out)
{
  return std::copy(it, it + count, out);
}
template<class Iterable, class OutputIterator> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterator_v<OutputIterator>, OutputIterator>
copy(const Iterable& c, OutputIterator out)
{
  return std::copy(c.begin(), c.end(), out);
}
template<class Iterable, class OutputIterator> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterator_v<OutputIterator>, OutputIterator>
copy(const Iterable& c, std::size_t count, OutputIterator out)
{
  return std::copy(c.begin(), c.begin() + count, out);
}
template<class Iterator, class OutputIterable> inline constexpr
std::enable_if_t<is_iterator_v<Iterator> && is_iterable_v<OutputIterable>, OutputIterable&>
copy(Iterator begin, Iterator end, OutputIterable& o)
{
  copy(begin, end, inserter(o));
  return o;
}
template<class Iterable, class OutputIterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<OutputIterable>, OutputIterable&>
copy(const Iterable& c, OutputIterable& o)
{
  return copy(c, inserter(o));
}
template<class Iterable, class Iterator> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterator_v<Iterator>, Iterable>
copy(Iterator begin, Iterator end)
{
  auto r = construct_with_size<Iterable>(static_cast<std::size_t>(end - begin));
  return copy(begin, end, r);
}
template<class Iterator, class OutputIterator, class UnaryPredicate> inline constexpr
std::enable_if_t<is_pointerlike_v<Iterator> && !is_iterable_v<Iterator>, OutputIterator>
copy_if(Iterator it, std::size_t count, OutputIterator out, UnaryPredicate f)
{
  return std::copy_if(it, it + count, out, f);
}
template<class Iterable, class OutputIterator, class UnaryPredicate> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, OutputIterator>
copy_if(const Iterable& c, OutputIterator out, UnaryPredicate f)
{
  return std::copy_if(c.begin(), c.end(), out, f);
}
template<class Iterable, class OutputIterator, class UnaryPredicate> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, OutputIterator>
copy_if(const Iterable& c, std::size_t count, OutputIterator out, UnaryPredicate f)
{
  return std::copy_if(c.begin(), count, out, f);
}

//
// has and has_if (not STL)
//
template<class Map> inline constexpr
std::enable_if_t<is_map_v<Map>, bool>
has(const Map& m, const typename Map::key_type& k)
{
  return find(m, k) != m.end();
}
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && !is_map_v<Iterable>
                 && is_equality_comparable_v<subtype<Iterable>, T>, bool>
has(const Iterable& c, const T& t)
{
  return find(c, t) != c.end();
}
template<class Iterable, class Unary> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_unary_function_v<Unary, subtype<Iterable>>, bool>
has_if(Iterable& c, Unary f)
{
  return find_if(c, f) != c.end();
}

//
// slice (not STL)
// TODO: rework pointer arithmetic for different iterator types
//
template<class Iterator, class IndexIterator, class OutputIterator> inline constexpr
OutputIterator
slice(Iterator begin, IndexIterator index_begin, IndexIterator index_end, OutputIterator out)
{
  for (; index_begin != index_end; ++index_begin, ++out) {
    *out = *(begin + *index_begin);
  }
  return out; 
}
template<class Iterable, class IterableIndices, class OutputIterator> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<IterableIndices>, OutputIterator>
slice(const Iterable& c, const IterableIndices& indices, OutputIterator out)
{
  return slice(c.begin(), indices.begin(), indices.end(), out);
}
template<class Iterable, class IterableIndices> inline constexpr
Iterable slice(const Iterable& c, const IterableIndices& indices) 
{
  auto r = construct_with_size<Iterable>(indices.size());
  slice(c, indices, inserter(r));
  return r;
}
 
//
// wrapping std::transform
// affect & apply - perform a function on every element with and without side effects
// Note:
// the last variation of transform and apply are in only different in that
// transform over (Iterable, Fn) is more general than apply
// because it allows changes of types
// because of this it may be wise to rename the last variation of transform
// apply, and thus avoid any confusion
// 
template<class Iterable, class OutputIterator, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterator_v<OutputIterator>, OutputIterator>
transform(const Iterable& c, iterator<const Iterable> end, OutputIterator out, Fn f)
{
  return std::transform(c.begin(), end, out, f);
}
template<class Iterator, class OutputIterator, class Fn> inline constexpr
std::enable_if_t<is_iterator_v<Iterator> && is_iterator_v<OutputIterator>, OutputIterator>
transform(Iterator it, std::size_t count, OutputIterator out, Fn f)
{
  return std::transform(it, it + count, out, f); 
}
template<class Iterable, class OutputIterator, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterator_v<OutputIterator>, OutputIterator>
transform(const Iterable& c, std::size_t count, OutputIterator out, Fn f)
{
  return transform(c.begin(), count, out, f); 
}
template<class Iterable, class OutputIterator, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && !is_iterable_v<OutputIterator>, OutputIterator>
transform(const Iterable& c, OutputIterator out, Fn f)
{
  return std::transform(c.begin(), c.end(), out, f); 
}
template<class Iterable, class OutputIterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<OutputIterable>, OutputIterable&>
transform(const Iterable& c, iterator<const Iterable> end, OutputIterable& out, Fn f)
{
  std::transform(c.begin(), end, inserter(out), f);
  return out;
}
template<class Iterator, class OutputIterable, class Fn> inline constexpr
std::enable_if_t<is_iterator_v<Iterator> && is_iterable_v<OutputIterable>, OutputIterable&>
transform(Iterator it, std::size_t count, OutputIterable& out, Fn f)
{
  std::transform(it, it + count, inserter(out), f); 
  return out;
}
template<class Iterable, class OutputIterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<OutputIterable>, OutputIterable&>
transform(const Iterable& c, std::size_t count, OutputIterable& out, Fn f)
{
  transform(c.begin(), count, inserter(out), f); 
  return out;
}
template<class Iterable, class OutputIterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<OutputIterable>, OutputIterable&>
transform(const Iterable& c, OutputIterable& out, Fn f)
{
  transform(c, inserter(out), f); 
  return out;
}
//
// Note the below won't work correctly for allocators or for dual-type containers such as map
// Another name for the final variant of transform below may be 'apply' while removing 
// the other apply version. Since apply is already used in 'std' maybe a wiser choice
// is for_each
//
template<template<class> class Iterable, class Fn, class... Ts> inline constexpr
std::enable_if_t<is_iterable_v<Iterable<Ts...>>, 
                 Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>>>
transform(const Iterable<Ts...>& c, Fn f)
{
  // auto r = construct_with_size(c.size(), Iterable<std::result_of_t<Fn(T)>>); 
  Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>> r{c.size()};
  return transform(c, r, f);
}
template<template<class> class Iterable, class Fn, class... Ts> inline constexpr
std::enable_if_t<is_iterable_v<Iterable<Ts...>>, 
                 Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>>>
transform(const Iterable<Ts...>& c, std::size_t count, Fn f)
{
  // auto r = construct_with_size(c.size(), Iterable<std::result_of_t<Fn(T)>>); 
  Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>> r{count};
  return transform(c, count, r, f);
}
template<template<class> class Iterable, class Fn, class... Ts> inline constexpr
std::enable_if_t<is_iterable_v<Iterable<Ts...>>, 
                 Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>>>
transform(const Iterable<Ts...>& c, iterator<Iterable<Ts...>> end, Fn f)
{
  // auto r = construct_with_size(c.size(), Iterable<std::result_of_t<Fn(T)>>); 
  Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>> r{end - c.begin()};
  return transform(c, end, r, f);
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
affect(Iterable& c, Fn f)
{
  transform(c, c.begin(), f);
  return c;
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable>
apply(Iterable c, Fn f)
{
  return affect(c, f);
}

//
// at and iterator_at - get (reference to) an element at an index or an iterator
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && !is_indexable_v<Iterable>, iterator<Iterable>>
iterator_at(Iterable& c, std::size_t index)
{
  // auto iter = c.begin();
  // for (int i = 0; i < index; ++i) std::next(iter);
  // return iter;
  return c.begin() + index;
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && !is_indexable_v<Iterable>, subtype<Iterable>&>
at(Iterable& c, std::size_t index)
{
  return *iterator_at(c, index);
}
// template<class Indexable> inline constexpr
// std::enable_if_t<is_indexable_v<Indexable>, iterator<Indexable>>
// at(Indexable& c, std::size_t index)
// {
//   return c[index]; 
// }

//
// common and statistical functions
// TODO: median, quartiles, percentiles
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
sum(const Iterable& c, const subtype<Iterable>& init = subtype<Iterable>{})
{
  return accumulate(c, 
    [](const subtype<Iterable>& t1, const subtype<Iterable>& t2) { return t1 + t2; });
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
product(const Iterable& c, const subtype<Iterable>& init = subtype<Iterable>{})
{
  return accumulate(c, 
    [](const subtype<Iterable>& t1, const subtype<Iterable>& t2) { return t1 * t2; });
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
mean(const Iterable& c)
{
  if (c.size() != 0) return sum(c) / c.size();
  return subtype<Iterable>{};
}

//
// 'centered_squares' - the (x_i - mu)^2 component used in calculating variance
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
affect_centered_squares(Iterable& c, const subtype<Iterable>& meanval)
{
  return affect(c, [meanval](subtype<Iterable> t) { return (t - meanval) * (t - meanval); });
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
affect_centered_squares(Iterable& c)
{
  return affect_centered_squares(c, mean(c));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, Iterable>
centered_squares(Iterable c, const subtype<Iterable>& meanval)
{
  return affect_centered_squares(c, meanval); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, Iterable>
centered_squares(const Iterable& c)
{
  return centered_squares(c, mean(c));
}

//
// variance and vol (vol is standard deviation)
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_variance(Iterable& c, const subtype<Iterable>& meanval)
{
  return mean(affect_centered_squares(c, meanval));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_variance(Iterable& c)
{
  return affect_variance(c, mean(c));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
variance(Iterable c, const subtype<Iterable>& meanval)
{
  return affect_variance(c, meanval);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
variance(const Iterable& c)
{
  return variance(c, mean(c));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
affect_vol(Iterable& c, const subtype<Iterable>& meanval)
{
  return sqrt(affect_variance(c, meanval));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
affect_vol(Iterable& c)
{
  return affect_vol(c, mean(c)); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
vol(Iterable c, const subtype<Iterable>& meanval)
{
  return affect_vol(c, meanval); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
vol(const Iterable& c)
{
  return vol(c, mean(c));
}

//
// median
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_median(Iterable& c)
{
  if (c.size() != 0) {
    sort(c);
    if (!is_even(c.size())) return at(c, c.size() / 2);
    else return (at(c, c.size() / 2) + at(c, c.size() / 2 + 1)) / 2; 
  }
  else return 0;
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
median(Iterable c)
{
  return affect_median(c);
}

} // namespace ryk

#endif
