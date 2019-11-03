#ifndef ryk_algorithm_extras
#define ryk_algorithm_extras

#include <stack>
#include <queue>

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
