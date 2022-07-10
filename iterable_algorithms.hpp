#ifndef ryk_iterable_algorithms
#define ryk_iterable_algorithms

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>

#include "traits.hpp"
#include "predicates.hpp"
#include "algorithm_extras.hpp"

namespace ryk {

//
// 20191205 TODO: the end of file has 'affect' and 'apply' as 'transfrom' variants.
// these probably need to be renamed. the 'at' functions at the end of file might not be useful.
//


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
// some of the below are commented out:
// I originally had index_of as a synonym of find_index
// but I think it's better to have index_of take an iterable and an iterator
// and return the index of the iterator.
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
  return std::distance(begin, std::find(begin, end, t));
}
// template<class Iterator, class T> inline constexpr
// size_t index_of(Iterator begin, Iterator end, const T& t)
// {
//   return find_index(begin, end, t);
// }
template<class Iterator, class Fn> inline constexpr
size_t find_if_index(Iterator begin, Iterator end, Fn f)
{
  return std::distance(begin, std::find_if(begin, end, f));
}
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
find_index(Iterable& c, const T& t)
{
  return std::distance(c.begin(), find(c, t));
}
template<class Iterable, class Pred> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
find_if_index(Iterable& c, Pred pred)
{
  return std::distance(c.begin(), find_if(c, pred));
}
template<class Iterable, class Pred> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
find_index_if(Iterable& c, Pred pred)
{
  return find_if_index(c, pred);
}
// The next two 'index_of's are commented out.
// I think index_of should just take an Iterable an iterator
// and return the index of the iterator.
//
// template<class Iterable, class T> inline constexpr
// std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
// index_of(Iterable& c, const T& t)
// {
//   return find_index(c, t);
// }
// template<class Iterable, class Pred> inline constexpr
// std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
// index_of_if(Iterable& c, Pred pred)
// {
//   return find_if_index(c, pred);
// }
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
index_of(const Iterable& c, const iterator<const Iterable> i)
{
  return std::distance(c.begin(), i);
}

//
// find_end - is std::search but returns the last match
//
template<class Iterable1, class Iterable2> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>, iterator<Iterable1>>
find_end(Iterable1& search_in, const Iterable2& search_for)
{
  return std::find_end(search_in.begin(), search_in.end(), search_for.begin(), search_for.end());
}
template<class Iterable1, class Iterable2, class BinaryFn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>
                 && is_binary_function_v<BinaryFn, subtype<Iterable1>, subtype<Iterable2>>, 
                 iterator<Iterable1>>
find_end(Iterable1& search_in, const Iterable2& search_for, BinaryFn f)
{
  return std::find_end(search_in.begin(), search_in.end(),
                       search_for.begin(), search_for.end(), f);
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
// mismatch - finds first mismatching pair of values in two iterables
//
template<class Iterable1, class Iterable2> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>, 
                 std::pair<iterator<Iterable1>, iterator<Iterable2>>>
mismatch(Iterable1& lhs, Iterable2& rhs)
{
  return std::mismatch(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template<class Iterable1, class Iterable2, class BinaryFn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>
                 && is_binary_function_v<BinaryFn, subtype<Iterable1>, subtype<Iterable2>>, 
                 std::pair<iterator<Iterable1>, iterator<Iterable2>>>
mismatch(Iterable1& lhs, Iterable2& rhs, BinaryFn f)
{
  return std::mismatch(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), f);
}

//
// equal - determines if the two iterables are equal 
//
template<class Iterable1, class Iterable2> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>, bool>
equal(Iterable1& lhs, Iterable2& rhs)
{
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template<class Iterable1, class Iterable2, class BinaryFn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>
                 && is_binary_function_v<BinaryFn, subtype<Iterable1>, subtype<Iterable2>>, 
                 bool>
equal(Iterable1& lhs, Iterable2& rhs, BinaryFn f)
{
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), f);
}

//
// accumulate and reduce (reduce not yet available in GCC <=9.2)
// please note that its probably proper to have the return type 
// of the second accumulate variation be:
//   std::result_of_t<Fn(T, subtype<Iterable>)>
// but the std::accumulate it calls just returns T, the type of 'init'

//
// Bringing std::accumulate functionality into ryk namespace - see next note
//
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
accumulate(Iterator first, Iterator last, std::decay_t<deref<Iterator>> init)
{
  return std::accumulate(first, last, init);
}
template<class Iterator, class BinaryFn> inline constexpr
std::enable_if_t<is_iterator_v<Iterator> && is_binary_function_v<BinaryFn>,
                 std::decay_t<deref<Iterator>>>
accumulate(Iterator first, Iterator last, std::decay_t<deref<Iterator>> init, BinaryFn f)
{
  return std::accumulate(first, last, init, f);
}
//
// A note for the next two signatures - the std:: signature kind of sucks. It requires there 
// to be an init value that is not within the sequence itself. 
// We typically don't actually want this. 
// The problem is the first signature is ambiguous with the std:: signature with 'T init'
// so have to explicity use ryk:: or std:: namespaces if bringing them both in.
//
template<class Iterator, class BinaryFn, 
         class Subtype = std::decay_t<deref<Iterator>>> inline constexpr
std::enable_if_t<is_iterator_v<Iterator> 
                 && is_binary_function_v<BinaryFn, Subtype, Subtype>, 
                 Subtype>
accumulate(Iterator first, Iterator last, BinaryFn f)
{
  if (first != last) {
    Subtype init = *first++;
    for (; first != last; ++first) init = f(std::move(init), *first);
    return init;
  } else return Subtype{};
}
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
accumulate(Iterator first, Iterator last)
{
  return ryk::accumulate(first, last, [](auto a, auto b){ return a + b; });
}
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> &&
                 !is_binary_function_v<T, subtype<Iterable>, subtype<Iterable>>, T>
accumulate(const Iterable& c, T init)
{
  return std::accumulate(c.begin(), c.end(), init);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
accumulate(const Iterable& c)
{
  return ryk::accumulate(c.begin(), c.end());
}
template<class Iterable, class T, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, T>
accumulate(const Iterable& c, T init, Fn f)
{
  return std::accumulate(c.begin(), c.end(), init, f);
}
template<class Iterable, class BinaryFn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> &&
                 is_binary_function_v<BinaryFn, subtype<Iterable>, subtype<Iterable>>,
                 subtype<Iterable>>
accumulate(const Iterable& c, BinaryFn f)
{
  return ryk::accumulate(c.begin(), c.end(), f);
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
template<class... Ts> inline constexpr
std::set<Ts...>& sort(std::set<Ts...>& s)
{
  return s;
}
template<class... Ts> inline constexpr
std::list<Ts...>& sort(std::list<Ts...>& l)
{
  l.sort();
  return l;
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
// replace, replace_if
// note that these functions replace all occurrences
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, void>
replace(Iterable& c, const subtype<Iterable>& old_value, const subtype<Iterable>& new_value)
{
  std::replace(c.begin(), c.end(), old_value, new_value);
}
//
// replace_if takes a UnaryPredicate to determine whether to replace an element
template<class Iterable, class Unary> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, void>
replace_if(Iterable& c, Unary pred, const subtype<Iterable>& new_value)
{
  std::replace_if(c.begin(), c.end(), pred, new_value);
}
//
// The next variants replace a value with any number of replacement values
// But they have a bug - they only replace the first occurrence
template<class Iterable, class InIter> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
replace(Iterable& c, const subtype<Iterable>& old_value, 
        InIter new_values_begin, InIter new_values_end)
{
  auto it = find(c, old_value);
  if (it != c.end()) return insert(c, it, new_values_begin, new_values_end);
  else return it;
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
template<class Iterable, class OutputIterator> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterator_v<OutputIterator>, OutputIterator>
copy(const Iterable& c, OutputIterator out)
{
  return std::copy(c.begin(), c.end(), out);
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
  return copy(c.begin(), c.end(), o);
}
//
// this one is basically just a constructor that takes 2 iterators
//
template<class Iterable, class Iterator> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterator_v<Iterator>, Iterable>
copy(Iterator begin, Iterator end)
{
  auto r = Iterable{};
  return copy(begin, end, r);
}
template<class Iterable, class OutputIterator, class UnaryPredicate> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, OutputIterator>
copy_if(const Iterable& c, OutputIterator out, UnaryPredicate f)
{
  return std::copy_if(c.begin(), c.end(), out, f);
}
template<class Iterable, class OutputIterable, class UnaryPredicate> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<OutputIterable>, OutputIterable&>
copy_if(const Iterable& c, OutputIterable& out, UnaryPredicate f)
{
  return std::copy_if(c, inserter(out), f);
}
template<class OutputIterable, class Iterable, class UnaryPredicate> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<OutputIterable>, OutputIterable&>
copy_if(const Iterable& c, UnaryPredicate f)
{
  auto r = OutputIterable{};
  return std::copy_if(c, r, f);
}

//
// shuffle
//
template<class Iterable, class URBG> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
shuffle(Iterable& c, URBG&& g)
{
  std::shuffle(c.begin(), c.end(), g);
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
//
template<class Iterator, class IndexIterator, class OutputIterator> inline constexpr
OutputIterator
slice(Iterator begin, IndexIterator index_begin, IndexIterator index_end, OutputIterator out)
{
  std::decay_t<deref<IndexIterator>> last_index = 0;
  for (; index_begin != index_end; ++index_begin, ++out) {
    std::advance(begin, *index_begin - last_index);
    last_index = *index_begin;
    *out = *begin;
    ++out;
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
  Iterable r;
  slice(c, indices, ryk::inserter(r));
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
std::enable_if_t<is_iterable_v<Iterable> && is_iterator_v<OutputIterator>, OutputIterator>
transform(const Iterable& c, OutputIterator out, Fn f)
{
  return std::transform(c.begin(), c.end(), out, f); 
}
template<class Iterator, class OutputIterable, class Fn> inline constexpr
std::enable_if_t<is_iterator_v<Iterator> && is_iterable_v<OutputIterable>, OutputIterable&>
transform(Iterator begin, Iterator end, OutputIterable& out, Fn f)
{
  std::transform(begin, end, inserter(out), f); 
  return out;
}
//
// Note the following is ambigious with std::transform() if both namespaces are brough in
// it must be explicity called with ryk::transform()
//
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
  Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>> r{};
  return transform(c, r, f);
}
template<template<class> class Iterable, class Fn, class... Ts> inline constexpr
std::enable_if_t<is_iterable_v<Iterable<Ts...>>, 
                 Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>>>
transform(const Iterable<Ts...>& c, std::size_t count, Fn f)
{
  // auto r = construct_with_size(c.size(), Iterable<std::result_of_t<Fn(T)>>); 
  Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>> r{};
  return transform(c, count, r, f);
}
template<template<class> class Iterable, class Fn, class... Ts> inline constexpr
std::enable_if_t<is_iterable_v<Iterable<Ts...>>, 
                 Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>>>
transform(const Iterable<Ts...>& c, iterator<Iterable<Ts...>> end, Fn f)
{
  // auto r = construct_with_size(c.size(), Iterable<std::result_of_t<Fn(T)>>); 
  // The following line is problematic
  // The Iterable<Ts..> suddenly loses all its extra type parameters
  Iterable<std::result_of_t<Fn(subtype<Iterable<Ts...>>)>> r{};
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
// iota
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
iota(Iterable& c, subtype<Iterable> init = subtype<Iterable>{})
{
  std::iota(c.begin(), c.end(), init);
  return c;
}

//
// lower_bound and upper_bound
// with their index_of_lower_bound & index_of_upper_bound variants
// index_of* variants return an index instead of an iterator
//
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_equality_comparable_v<subtype<Iterable>, T>,
                 iterator<Iterable>>
lower_bound(Iterable& c, const T& t)
{
  return std::lower_bound(c.begin(), c.end(), t);
}
template<class Iterable, class T, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>,
                 iterator<Iterable>>
lower_bound(Iterable& c, const T& t, Compare comp)
{
  return std::lower_bound(c.begin(), c.end(), t, comp);
}
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_equality_comparable_v<subtype<Iterable>, T>,
                 distance<Iterable>>
index_of_lower_bound(Iterable& c, const T& t)
{
  return index_of(c, lower_bound(c, t));
}
template<class Iterable, class T, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>,
                 distance<Iterable>>
index_of_lower_bound(Iterable& c, const T& t, Compare comp)
{
  return index_of(c, lower_bound(c, t, comp)); 
}
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_equality_comparable_v<subtype<Iterable>, T>,
                 iterator<Iterable>>
upper_bound(Iterable& c, const T& t)
{
  return std::upper_bound(c.begin(), c.end(), t);
}
template<class Iterable, class T, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>,
                 iterator<Iterable>>
upper_bound(Iterable& c, const T& t, Compare comp)
{
  return std::upper_bound(c.begin(), c.end(), t, comp);
}
template<class Iterable, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_equality_comparable_v<subtype<Iterable>, T>,
                 distance<Iterable>>
index_of_upper_bound(Iterable& c, const T& t)
{
  return index_of(c, upper_bound(c, t));
}
template<class Iterable, class T, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>,
                 distance<Iterable>>
index_of_upper_bound(Iterable& c, const T& t, Compare comp)
{
  return index_of(c, upper_bound(c, t, comp));
}

//
// print (not STL)
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, std::ostream&>
print(const Iterable& c, std::ostream& os)
{
  ryk::copy(c, std::ostream_iterator<subtype<Iterable>>(os, " "));
  return os;
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, std::ostream&>
print(const Iterable& c)
{
  return print(c, std::cout);
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
// match (not STL, equivalent of Excel VLOOKUP)
// Given a sorted Iterable 'search_fields', match returns the value
// of a corresponding Iterable. The index of the corresponding Iterable
// is the upper bound of a search criterion within the search_fields Iterable. 
template<class Iterable1, class Iterable2, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>, iterator<Iterable2>>
match(const Iterable1& search_fields, const Iterable2& return_values, const T& criterion)
{
  return ryk::at(return_values, ryk::index_of_upper_bound(search_fields, criterion));
}
template<class Iterable1, class Iterable2, class T> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>, iterator<Iterable2>>
exact_match(const Iterable1& search_fields, const Iterable2& return_values, const T& criterion)
{
  return ryk::at(return_values, ryk::find_index(search_fields, criterion));
}

} // namespace ryk

#endif
