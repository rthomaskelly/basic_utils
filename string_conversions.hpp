#ifndef ryk_string_conversions_hpp
#define ryk_string_conversions_hpp

#include "traits.hpp"
#include "ostream_operators.hpp"
#include "string_predicates.hpp"

#include <string>
#include <memory>
#include <variant>

namespace ryk {

//
// to_string functions for some types not included in std:: to give fuller coverage
// (const char*, std::string, char, bool, shared/unqiue ptr, Iterables)
// includes wrappers around std::to_string functions so everything is in one namespace 
// also includes as_string() synonyms
//

//
// const char *
//
inline std::string to_string(const char* s)
{ 
  return string(s);
}
inline std::string as_string(const char* s)
{
  return to_string(s);
}

//
// std::string (is identity function)
//
inline std::string to_string(const std::string& s) 
{ 
  return s; 
}
inline std::string as_string(const std::string& s) 
{ 
  return to_string(s); 
}

//
// char
//
inline std::string to_string(const char c)
{ 
  return string(1, c);
}
inline std::string as_string(const char c)
{ 
  return to_string(c);
}

//
// bool
//
inline std::string to_string(const bool b)
{ 
  return b ? "true" : "false";
}
inline std::string as_string(const bool b)
{ 
  return to_string(b);
}

//
// int (std:: wrapper)
//
inline std::string to_string(const int i)
{ 
  return std::to_string(i);
}
inline std::string as_string(const int i)
{ 
  return to_string(i);
}

//
// uint (std:: wrapper)
//
inline std::string to_string(const unsigned int n)
{ 
  return std::to_string(n);
}
inline std::string as_string(const unsigned int n)
{ 
  return to_string(n);
}

//
// double (std:: wrapper)
//
inline std::string to_string(const double d)
{ 
  return std::to_string(d);
}
inline std::string as_string(const double d)
{ 
  return to_string(d);
}

//
// shared_ptr
//
template<class T> std::string to_string(const std::shared_ptr<T>& ptr)
{ 
  return to_string(*ptr);
}
template<class T> std::string as_string(const std::shared_ptr<T>& ptr)
{ 
  return to_string(ptr);
}

//
// unique_ptr
//
template<class T> std::string to_string(const std::unique_ptr<T> ptr)
{ 
  return to_string(*ptr);
}
template<class T> std::string as_string(const std::unique_ptr<T> ptr)
{ 
  return to_string(ptr);
}

//
// Iterable (non-map)
//
template<class Iterable>
std::enable_if_t<is_iterable_v<Iterable> && !is_map_v<Iterable> && !is_string_v<Iterable>,
                 std::string> constexpr inline
to_string(const Iterable& c, std::string sep = " ")
{
  std::ostringstream oss;
  for (auto it = c.begin(); it != c.end(); ++it) {
    oss << *it;
    if (it != c.end() - 1) oss << sep;
  }
  return oss.str(); 
}
template<class Iterable>
std::enable_if_t<is_iterable_v<Iterable> && !is_map_v<Iterable> && !is_string_v<Iterable>,
                 std::string> constexpr inline
as_string(const Iterable& c, std::string sep = " ")
{
  return to_string(c, sep);
}

// to_number
std::variant<int, double> to_number(std::string s)
{
  if (!is_number(s)) 
    throw std::runtime_error("Tried to convert string '" + s + "' to_number().");
  else if (is_double(s)) return std::stod(s);
  else return std::stoi(s);
}

} // namespace ryk

#endif
