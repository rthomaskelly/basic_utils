#ifndef ryk_ostream_operators
#define ryk_ostream_operators

#include <fstream>
#include "iterable_algorithms.hpp"

namespace ryk {

template<class Iterable, class... Ts, 
         enable_if_p<is_iterable_v<Iterable> && !is_map_v<Iterable>>...> inline
std::ostream& operator<<(std::ostream& os, const Iterable& c)
{
  for (auto it = c.begin(); it != c.end() - 1; ++it) os << *it << " ";
  os << *it; 
  return os;
}
template<class Map, class... Ts, 
         enable_if_p<is_iterable_v<Map> && is_map_v<Map>>...> inline
std::ostream& operator<<(std::ostream& os, const Map& m)
{
  for (auto it = c.begin(); it != c.end() - 1; ++it)
    os << it->first << " : " << it->second << ", ";
  os << it->first << " : " << it->second; 
  return os;
}


} // namespace ryk

#endif
