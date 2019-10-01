#ifndef ryk_iterable_utils
#define ryk_iterable_utils

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
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && !is_map_v<Iterable>, iterator<Iterable>>
find(Iterable& c, const subtype<Iterable>& t)
{
  return std::find(c.begin(), c.end(), t);
}
template<class Iterable, class Pred> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, iterator<Iterable>>
find_if(Iterable& c, Pred pred)
{
  return std::find_if(c.begin(), c.end(), pred);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
find_index(Iterable& c, const subtype<Iterable>& t)
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
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
index_of(Iterable& c, const subtype<Iterable>& t)
{
  return find_index(c, t);
}
template<class Iterable, class Pred> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, distance<Iterable>>
index_of_if(Iterable& c, Pred pred)
{
  return find_if_index(c, pred);
}

//
// accumulate
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
accumulate(const Iterable& c, subtype<Iterable> init = subtype<Iterable>{})
{
  return std::accumulate(c.begin(), c.end(), init);
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
accumulate(const Iterable& c, subtype<Iterable> init, Fn f)
{
  return std::accumulate(c.begin(), c.end(), init, f);
}

//
// sort
//
template<class T> inline constexpr
void sort(const std::set<T>& s)
{
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, void>
sort(Iterable& c)
{
  std::sort(c.begin(), c.end());
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, void>
sort(Iterable& c, Compare compare)
{
  std::sort(c.begin(), c.end(), compare);
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
  replace(c, old_value, i.begin(), i.end());
}

//
// unique and erase_duplicates
// erase_duplicates is the function that will actually erase the duplicates
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
unique(Iterable& c)
{
  std::unique(c.begin(), c.end());
}
template<class Iterable, class Compare> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, iterator<Iterable>>
unique(Iterable& c, Compare compare)
{
  std::unique(c.begin(), c.end(), compare);
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
std::enable_if_t<is_iterable_v<Iterable>, void>
reverse(Iterable& c)
{
  std::reverse(c.begin(), c.end());
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
// has and has_if (not STL)
//
template<class Map> inline constexpr
std::enable_if_t<is_map_v<Map>, bool>
has(const Map& m, const typename Map::key_type& k)
{
  return find(m, k) != m.end();
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && !is_map_v<Iterable>, bool>
has(const Iterable& c, const subtype<Iterable>& t)
{
  return find(c, t) != c.end();
}
template<class Iterable, class Pred> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, bool>
has_if(Iterable& c, Pred pred)
{
  return find_if(c, pred) != c.end();
}

//
// useful iterable functions common in other languages
// note: that reduce() is a lot like std::accumulate but takes a
// function argument before an initial value allowing default initial values
// also: the name std::accumulate is a poor choice being a synonym for sum
// the name 'reduce' is the original Lisp name
//

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
// affect & apply - perform a function on every element with and without side effects
//
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, Iterable&>
affect(Iterable& c, Fn f)
{
  for (auto& t : c) t = f(std::move(t));
  return c;
}
template<class Iterable, class Fn> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, Iterable>
apply(Iterable c, Fn f)
{
  return affect(c, f);
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
// reduce is like std::accumulate
//
template<class Iterable, class F> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
reduce(const Iterable& c, F f, subtype<Iterable> init = subtype<Iterable>{})
{
  for (const auto& t : c) init = f(std::move(init), t);
  return init;
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
  return reduce(c, 
    [](const subtype<Iterable>& t1, const subtype<Iterable>& t2) { return t1 + t2; });
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
product(const Iterable& c, const subtype<Iterable>& init = subtype<Iterable>{})
{
  return reduce(c, 
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

//
// experimenting from here on
//

//
// circle_index implements python-like negative indices
//
template<class Sizeable> inline constexpr
subtype<Sizeable> circle_index(Sizeable& c, int64_t index)
{
  // the following is equivalent to:
  //   if (index < 0) index + c.size();
  //   else index;
  // but the bit-wise operations and integer multiplication s/b done in a few cycles

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
template<class T, template<class> class Iterable1, template<class> class Iterable2> inline constexpr
std::enable_if_t<is_iterable<Iterable1<T>>::value && is_iterable<Iterable2<uint64_t>>::value, Iterable2<T>>
slice(const Iterable1<T>& c, const Iterable2<uint64_t> indices) {
  Iterable2<T> ret;
  // for (uint64_t n : indices) ret[
  return ret;
}
template<class T, template<class, class alloc = std::allocator<T>> class Iterable, class Pred>
std::enable_if_t<is_iterable<Iterable<T>>::value, std::vector<T>>
select(const Iterable<T>& c, Pred p) {
  std::vector<T> ret;
  for (auto& each : c) if (p(each)) ret.push_back(each);
  return ret;
}

// define push:
// push(container, T) -> container::iterator
// for vector does pushback
// for array does pushback (requires keeping track of sizeused)
// for list appends back
// for map, hashmap does insert
// for set(sorted unique vector) does insert
// returns an iterator




} // namespace ryk

#endif