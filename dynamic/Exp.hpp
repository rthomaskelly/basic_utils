#pragma once

#include <string>
#include <variant>
#include <boost/lexical_cast.hpp>

#include <basic_utils/dynamic/dlist.hpp>
#include "RTA.hpp"

namespace ryk {

using List = ryk::dlist;
using ExpT = std::variant<RTA, List>;

struct Exp 
{
  Exp(RTA a)
   : value(a), type(ExpType::Atom)
  {}
  
  Exp(List l)
   : value(l), type(ExpType::List)
  {}
 
  ExpT value;
  ExpType type;

  const RTA& getAtom() const
  {
    if (type == ExpType::Atom) return std::get<RTA>(value);
    else throw std::runtime_error("Tried to getAtom from Exp that was not an Atom.");
  }
  RTA& getAtom()
  {
    return const_cast<RTA&>(const_cast<const Exp*>(this)->getAtom());
  }

  const List& getList() const
  {
    if (type == ExpType::List) return std::get<List>(value);
    else throw std::runtime_error("Tried to getList from Exp that was not a List.");
  }
  List& getList()
  {
    return const_cast<List&>(const_cast<const Exp*>(this)->getList());
  }

};

std::ostream& operator<<(std::ostream& os, const Exp& x)
{
  switch (x.type) {
    case ExpType::Atom: {
      os << x.getAtom();
      break;
    }
    case ExpType::List: {
      os << x.getList();      
      break;
    }
    default: throw std::runtime_error("ostream operator<< on invalid Exp.");
  }
  return os;
}

//
// RTTI ostream operator
inline std::ostream& operator<<(std::ostream& os, const RTTI& t)
{
  switch (t.type()) {
    case ExpType::Atom: {
      // could just reinterpret cast here also and remove the virtual dtor in RTTI
      os << dynamic_cast<const RTA&>(t);
      break;
    }
    case ExpType::List: {
      os << dynamic_cast<const dlist&>(t);
      break;
    }
    default: {
      throw std::runtime_error("ostream operator<< on invalid RTTI.");
      break;
    }
  }
  return os;
}

//
// dlist ostream operator
inline std::ostream& operator<<(std::ostream& os, const dlist& l)
{
  os << "[ ";
  for (auto& e : l) os << e.data<RTTI>() << " ";
  os << "]";
  return os; 
}

} // namespace ryk

namespace std {
inline string to_string(const ryk::Exp& e)
{
  return boost::lexical_cast<string>(e);
}
inline string to_string(const ryk::dlist& l)
{
  return boost::lexical_cast<string>(l);
}
inline string to_string(const ryk::RTTI& r)
{
  return boost::lexical_cast<string>(r);
}
} // namespace std

