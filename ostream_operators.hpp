#ifndef ryk_ostream_operators
#define ryk_ostream_operators

#include <fstream>
#include "iterable_algorithms.hpp"

namespace ryk {

template<class Iterable, 
         enable_if_p<is_iterable_v<Iterable> && !is_map_v<Iterable>
                     && !is_string_v<Iterable>>...> inline
std::ostream& operator<<(std::ostream& os, const Iterable& c)
{
  //for (auto it = c.begin(); it != c.end(); ++it) {
  //  os << *it;
  //  if (it != c.end() - 1) os << " ";
  //}
  print(c);
  return os;
}
template<class Map, 
         enable_if_p<is_iterable_v<Map> && is_map_v<Map>>...> inline
std::ostream& operator<<(std::ostream& os, const Map& m)
{
  for (auto it = m.begin(); it != m.end(); ++it) {
    os << it->first << " : " << it->second; 
    if (it != m.end() - 1) os << ", ";
  }
  return os;
}


} // namespace ryk

#endif
