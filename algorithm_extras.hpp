#ifndef ryk_algorithm_extras
#define ryk_algorithm_extras

#include <vector>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <set>
#include <map>

#include "traits.hpp"

namespace ryk {
  
//
// head() definitions provides uniform interface
// to get the top() of stack or front() of queue
//
template<class T> inline
T& head(std::stack<T>& s)
{
  return s.top();
}
template<class T> inline
T& head(std::queue<T>& s)
{
  return s.front();
}

//
// pop() definition pops and returns
//
template<class T> inline
T pop(std::stack<T>& s)
{
  T t = head(s);
  s.pop();
  return t;
}
template<class T> inline
T pop(std::queue<T>& q)
{
  T t = head(q);
  q.pop();
  return t; 
}
template<class T> inline
T pop(std::vector<T>& v)
{
  T t = v.back();
  v.pop_back();
  return t;
}

//
// push() definitions can push iterables
//
template<class T> inline
void push(std::stack<T>& s, const T& t)
{
  s.push(t);
}
template<class T> inline
void push(std::queue<T>& q, const T& t)
{
  q.push(t);
}
template<class T> inline
void push(std::vector<T>& v, const T& t)
{
  v.push_back(t);
}
// template<class Container, class Iterable> inline
// std::enable_if_t<is_iterable_v<Iterable>, void>
// void push(Container& c, const Iterable& i)
// {
//   for (auto& e : i) push(c, e);
// }

//
// value_of - gets the value either by derefercing an iterator or
//            deref'ing the pair iterator and taking .second
//
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator> && !is_pair_iterator_v<Iterator>, deref<Iterator>>
value_of(Iterator it)
{
  return *it;
}
template<class Iterator> inline constexpr
std::enable_if_t<is_pair_iterator_v<Iterator>, deref<Iterator>>
value_of(Iterator it)
{
  return it->second;
}


//
// inserter() definitions return an insert iterator based on the container
//
template<class... Ts> inline
std::back_insert_iterator<std::vector<Ts...>> inserter(std::vector<Ts...>& v)
{
  return std::back_inserter(v);
}
inline
std::back_insert_iterator<std::string> inserter(std::string& s)
{
  return std::back_inserter(s);
}
template<class... Ts> inline
std::back_insert_iterator<std::list<Ts...>> inserter(std::list<Ts...>& m)
{
  return std::back_inserter(m);
}
template<class... Ts> inline
std::front_insert_iterator<std::forward_list<Ts...>> inserter(std::forward_list<Ts...>& m)
{
  return std::front_inserter(m);
}
template<class... Ts> inline
std::back_insert_iterator<std::deque<Ts...>> inserter(std::deque<Ts...>& d)
{
  return std::back_inserter(d);
}
template<class... Ts> inline
std::insert_iterator<std::set<Ts...>> inserter(std::set<Ts...>& s)
{
  return std::inserter(s, s.end());
}
template<class... Ts> inline
std::insert_iterator<std::multiset<Ts...>> inserter(std::multiset<Ts...>& s)
{
  return std::inserter(s, s.end());
}
template<class... Ts> inline
std::insert_iterator<std::map<Ts...>> inserter(std::map<Ts...>& m)
{
  return std::inserter(m, m.end());
}
template<class... Ts> inline
std::insert_iterator<std::unordered_map<Ts...>> inserter(std::unordered_map<Ts...>& m)
{
  return std::inserter(m, m.end());
}

//
// construct_with_size
// sequence containers std::vector, std::deque, std::forward_list and std::list
// all have by-size constructors, we'd like to take advantage of those when 
// creating a new container generically
template<template<class> class Iterable, class... Ts>
struct is_sequence
{
  static constexpr bool value = is_iterable_v<Iterable> && 
                                (std::is_same_v<Iterable<Ts...>, std::vector<Ts...>> ||
                                std::is_same_v<Iterable<Ts...>, std::deque<Ts...>> ||
                                std::is_same_v<Iterable<Ts...>, std::list<Ts...>> ||
                                std::is_same_v<Iterable<Ts...>, std::forward_list<Ts...>>);
};
template<template<class> class Iterable, class... Ts>
static constexpr bool is_sequence_v = is_sequence<Iterable, Ts...>::value;

template<template<class> class Iterable, class... Ts>
struct is_hashmap
{
  static constexpr bool value = is_iterable_v<Iterable> && 
                              std::is_same_v<Iterable<Ts...>, std::unordered_map<Ts...>>;
};
template<template<class> class Iterable, class... Ts>
static constexpr bool is_hashmap_v = is_hashmap<Iterable, Ts...>::value;

// template<class Sequence> inline constexpr
// std::enable_if_t<is_sequence_v<Sequence>, Sequence>
// construct_with_size(std::size_t size)
// {
//  return Sequence{size};
// }

// template<class... Ts> inline
// std::vector<Ts...> construct_with_size(std::size_t size, std::vector<Ts...>)
// {
//   return std::vector<Ts...>{size};
// }
// template<class... Ts> inline
// std::deque<Ts...> construct_with_size(std::size_t size, std::deque<Ts...>)
// {
//   return std::deque<Ts...>{size};
// }
// template<class... Ts> inline
// std::list<Ts...> construct_with_size(std::size_t size, std::list<Ts...>)
// {
//   return std::list<Ts...>{size};
// }
// template<class... Ts> inline
// std::forward_list<Ts...> construct_with_size(std::size_t size, std::forward_list<Ts...>)
// {
//   return std::forward_list<Ts...>{size};
// }
template<class... Ts> inline
std::unordered_map<Ts...> construct_with_size(std::size_t size, std::unordered_map<Ts...>)
{
  return std::unordered_map<Ts...>{size};
}
// template<template<class> class Iterable, class... Ts> inline
// std::enable_if_t<!is_sequence_v<Iterable, Ts...> && !is_hashmap_v<Iterable, Ts...>,
//                  Iterable<Ts...>>
// construct_with_size(std::size_t size, Iterable<Ts...>)
// {
//   return Iterable<Ts...>{};
// }


//
// insert
//
template<class Insertable> inline
std::enable_if_t<is_insertable_v<Insertable>, iterator<Insertable>>
insert(Insertable& c, iterator<Insertable> pos, const subtype<Insertable>& t)
{
  return c.insert(pos, t);
}
template<class Insertable, class InIter> inline
std::enable_if_t<is_insertable_v<Insertable>, iterator<Insertable>>
insert(Insertable& c, iterator<Insertable> pos,
       InIter new_values_begin, InIter new_values_end)
{
  return c.insert(pos, new_values_begin, new_values_end);
}
template<class Insertable, class Iterable> inline
std::enable_if_t<is_insertable_v<Insertable> && is_iterable_v<Iterable>, iterator<Insertable>>
insert(Insertable& c, iterator<Insertable> pos, const Iterable& new_values)
{
  return insert(c, pos, new_values.begin(), new_values.end());
}

//
// append
//
template<class Insertable, class Iterable> inline
std::enable_if_t<is_insertable_v<Insertable> && is_iterable_v<Iterable>, iterator<Insertable>>
append(Insertable& c, const Iterable& new_values)
{
  return insert(c, c.end(), new_values);
}

//
// keys - returns a vector of all the keys of a map
// values - returns a vector of all the values of a map
// TODO: this should be replaed by a view
//
template<class Map, enable_if_p<is_map_v<Map>>...> inline constexpr
std::vector<typename Map::key_type> keys(const Map& m)
{
  auto r = std::vector<typename Map::key_type>{m.size()};
  for (auto& p : m) r.push_back(p.first);
  return r;
}
template<class Map, enable_if_p<is_map_v<Map>>...> inline constexpr
std::vector<typename Map::key_type> values(const Map& m)
{
  auto r = std::vector<typename Map::key_type>{m.size()};
  for (auto& p : m) r.push_back(p.second);
  return r;
}

} // namespace ryk

#endif
