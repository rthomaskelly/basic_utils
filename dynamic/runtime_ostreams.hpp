#pragma once

#include <fstream>
#include "Exp.hpp"

namespace ryk {

std::ostream& operator<<(std::ostream& os, const Exp& x)
{
  switch (x.subtype()) {
    case ExpType::Atom: {
      os << x.getAtom();
      break;
    }
    case ExpType::List: {
      os << x.getList();
      break;
    }
    default: throw std::runtime_error("ostream operator<< on invalid Exp."
               "Exp has type '" + std::to_string(x.subtype()) + "'.");
  }
  return os;
}

//
// RTTI ostream operator
inline std::ostream& operator<<(std::ostream& os, const RTTI& t)
{
  switch (t.type()) {
    case ExpType::Atom: {
      os << dynamic_cast<const RTA&>(t);
      break;
    }
    case ExpType::List: {
      os << dynamic_cast<const List&>(t);
      break;
    }
    case ExpType::Exp: {
      os << dynamic_cast<const Exp&>(t);
      break;
    }
    default: {
      throw std::runtime_error("ostream operator<< on invalid RTTI." 
             " Has type value of '" + std::to_string(t.type()) + "'.");
      break;
    }
  }
  return os;
}

//
// runtime_list ostream operator
inline std::ostream& operator<<(std::ostream& os, const runtime_list& l)
{
  os << "[ ";
  for (auto e : l) os << *e << " ";
  os << "]";
  return os;
}


//
// dlist ostream operator
// inline std::ostream& operator<<(std::ostream& os, const dlist& l)
// {
//   os << "[ ";
//   for (auto& e : l) os << e.data<RTTI>() << " ";
//   os << "]";
//   return os; 
// }

} // namespace ryk

namespace std {
inline string to_string(const ryk::Exp& e)
{
  return boost::lexical_cast<string>(e);
}
inline string to_string(const ryk::runtime_list& l)
{
  return boost::lexical_cast<string>(l);
}
// inline string to_string(const ryk::dlist& l)
// {
//   return boost::lexical_cast<string>(l);
// }
inline string to_string(const ryk::RTTI& r)
{
  return boost::lexical_cast<string>(r);
}
} // namespace std

