#ifndef ryk_haskell
#define ryk_haskell

#include "iterable_algorithms.hpp"

namespace ryk {

//
// take, gives the first n elements
//
template<class Iterable, class OutputIterator> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterator_v<OutputIterator>, OutputIterator>
take(const Iterable& c, std::size_t n, OutputIterator o)
{
  return std::copy(c.begin(), c.begin() + n, o);
}
template<class Iterable, class OutputIterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<OutputIterable>, OutputIterable&>
take(const Iterable& c, std::size_t n, OutputIterable& o)
{
  return take(c, n, o.begin());
}
template<class OutputIterable, class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable> && is_iterable_v<OutputIterable>, OutputIterable>
take(const Iterable& c, std::size_t n)
{
  OutputIterable o;
  return take(c, n, o.begin());
}
// template<class Iterable> inline constexpr
// std::enable_if_t<is_iterable_v<Iterable>, Iterable>
// take(const Iterable& c, std::size_t n)
// {
//  return take<Iterable>(c, n);
// }

template<class Iterable1, class Iterable2> inline constexpr
std::enable_if_t<is_iterable_v<Iterable1> && is_iterable_v<Iterable2>, 
                 std::vector<std::tuple<subtype<Iterable1>, subtype<Iterable2>>>>
zip(const Iterable1& c1, const Iterable2& c2)
{
  std::vector<std::tuple<subtype<Iterable1>, subtype<Iterable2>>> r;
  r.reserve(std::min(c1.size(), c2.size()));
  auto i1 = c1.begin();
  auto i2 = c2.begin();
  for (; i1 != c1.end() && i2 != c2.end(); ++i1, ++i2) {
    r.emplace_back(*i1, *i2);
  } 
  return r;
}


} // namespace ryk

#endif
