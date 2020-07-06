#pragma once

#include "Exp.hpp"

namespace ryk {

template<class T>
constexpr ExpType type_to_enum()
{
  if constexpr (std::is_same_v<T, RTA>) return ExpType::Atom;
  else if constexpr (std::is_same_v<T, List>) return ExpType::List;
  else if constexpr (std::is_same_v<T, Exp>) return ExpType::Exp;
  else static_assert("Invalid type T is not RTA, List or Exp in type_to_enum().");
}

inline void check_front(List& l, ExpType type, 
              std::string empty_error, std::string wrong_type_error)
{ 
  if (empty_error.empty()) empty_error = "Empty List when calling get() for type '"
                                         + std::to_string(type) + "'.";
  if (l.empty()) throw std::runtime_error(empty_error);
  if (wrong_type_error.empty()) wrong_type_error = "Front of list has wrong type in get() call."
             " Has type '" + std::to_string(l.front().type()) + "' but expecting type '"
             + std::to_string(type) + "'.";
  if (l.front().type() != type) throw std::runtime_error(wrong_type_error);
}
template<class T> inline
std::enable_if_t<std::is_base_of_v<RTTI, T>, T>
checked_front(List& l)
{
  using namespace std::string_literals;
  check_front(l, type_to_enum<T>(), ""s, ""s); 
  return l.front<T>();
}
template<class T> inline
std::enable_if_t<std::is_base_of_v<RTTI, T>, T>
checked_pop(List& l)
{
  using namespace std::string_literals;
  check_front(l, type_to_enum<T>(), ""s, ""s); 
  T t = l.front<T>(); 
  l.pop_front();
  return t;
}

} // namespace ryk
