#ifndef ryk_string_conversions_hpp
#define ryk_string_conversions_hpp

#include "traits.hpp"
#include "ostream_operators.hpp"

#include <string>
#include <memory>
#include <boost/lexical_cast.hpp>

namespace ryk {

inline std::string as_string(const char* s)
{ 
  return string(s);
}
inline std::string as_string(const std::string& s) 
{ 
  return s; 
}
inline std::string as_string(const char c)
{ 
  return string(1, c);
}
inline std::string as_string(const bool b)
{ 
  return b ? "true" : "false";
}
inline std::string as_string(const int i)
{ 
  return std::to_string(i);
}
inline std::string as_string(const unsigned int n)
{ 
  return std::to_string(n);
}
inline std::string as_string(const double d)
{ 
  return std::to_string(d);
}
template<class T> std::string as_string(const std::shared_ptr<T>& ptr)
{ 
  return as_string(*ptr);
}
template<class Iterable>
std::enable_if_t<is_iterable_v<Iterable> && !is_map_v<Iterable> && !is_string_v<Iterable>,
                 std::string> constexpr inline
as_string(const Iterable& c, std::string sep = " ")
{
  std::ostringstream oss;
  for (auto it = c.begin(); it != c.end(); ++it) {
    oss << *it;
    if (it != c.end() - 1) oss << sep;
  }
  return oss.str(); 
}

} // namespace ryk

#endif
