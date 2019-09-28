#ifndef ryk_ostream_operators
#define ryk_ostream_operators

#include <fstream>
#include "iterable_utils.hpp"

namespace ryk {

// template<class Iterable, enable_if_p<is_iterable_v<Iterable>>...> inline
// std::ostream& operator<<(std::ostream& os, const Iterable& c)
// {
//   for (auto& e : c) os << e << " ";
//   return os;
// }
template<class... Ts> inline
std::ostream& operator<<(std::ostream& os, const std::vector<Ts...>& c)
{
  for (auto& e : c) os << e << " ";
  return os;
}

} // namespace ryk

#endif
