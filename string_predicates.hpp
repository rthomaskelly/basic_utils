#ifndef ryk_string_predicates
#define ryk_string_predicates

#include <cctype>
#include <string>

#include "iterable_algorithms.hpp"

using std::string;

namespace ryk {

// This file contains quite a few synonyms for a given definition.
// The reason for this is C made many predicates using the notation is___
// i.e. isdigit, isspace, having no underbar between is and the word
// which looks a bit odd. Further Lisp offers predicates with the word
// suffixed with -p. i.e. digitp spacep.
// We'll offer all these choices.

template<class T>
struct is_string
{
  static constexpr bool value = is_iterable_v<T> && std::is_same_v<subtype<const T>, const char>;
};
template<class T> constexpr inline bool is_string_v = is_string<T>::value;

//
// is_uint(string) and its synonyms
//
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
is_uint(const String& s)
{
  return !s.empty() && all_of(s, ::isdigit); 
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
isuint(const String& s)
{
  return is_uint(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
uintp(const String& s)
{
  return is_uint(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
is_nat(const String& s)
{
  return is_uint(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
isnat(const String& s)
{
  return is_uint(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
natp(const String& s)
{
  return is_uint(s);
}

//
// is_int(string) and its synonyms 
//
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
is_int(const String& s)
{
  if (is_nat(s)) return true;
  if (s.size() >= 2 && s[0] == '-') return is_nat(s.substr(1));
  return false;
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
isint(const String& s)
{
  return is_int(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
intp(const String& s)
{
  return is_int(s);
}

//
// is_float(string) and its synonyms
//
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
is_float(const String& s)
{
  // some test cases:
  // '-.' '-' '.' '..' '.-.' '--.'
  // '-a.' '$a.' 'a-.' 'a' 
  // '2-.' '2..' '.2.' '2-' 
  // '2' '2.' '-2.' '22.22' '-22.22'
  bool dot = false, dash = false, digit = false;
  for (auto rit = s.rbegin(); rit != s.rend(); ++rit) {
    if (*rit == '.' && dot) return false;
    else if (*rit == '.') dot = true;
    else if (*rit == '-' && !digit) return false; 
    else if (*rit == '-') dash = true;
    else if (dash && !isspace(*rit)) return false;
    else if (!isdigit(*rit)) return false;
    else digit = true;
  } 
  return true;
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
is_double(const String& s)
{
  return is_float(s);
}

//
// is_number(string) is synonymous with is_float
//
template<class String, enable_if_p<is_string_v<String>>...> inline
bool is_number(const String& s)
{
  return is_float(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
is_numeric(const String& s)
{
  return is_number(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
isnumber(const String& s)
{
  return is_number(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
isnumeric(const String& s)
{
  return is_number(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
numberp(const String& s)
{
  return is_number(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
numericp(const String& s)
{
  return is_number(s);
}

//
// has_space(string) and its synonyms
//
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
has_space(const String& s)
{
  return has_if(s, ::isspace);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
hasspace(const String& s)
{
  return has_space(s);
}
template<class String> inline
std::enable_if_t<is_string_v<String>, bool>
spacep(const String& s)
{
  return has_space(s);
}

} // namespace ryk

#endif
