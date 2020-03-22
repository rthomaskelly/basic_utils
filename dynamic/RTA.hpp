#pragma once

#include <string>
#include <variant>
#include <boost/lexical_cast.hpp>

#include "RTN.hpp" 

namespace ryk {

using Symbol = std::string;
using Atom = std::variant<Symbol, Number>;

class RTA : public ryk::RTTI
{
 public:
  RTA(int i)
   : RTTI(ExpType::Atom), value(i), atomic_type(AtomicType::Int)
  {}

  RTA(double d)
   : RTTI(ExpType::Atom), value(d), atomic_type(AtomicType::Double)
  {}

  RTA(Symbol s)
   : RTTI(ExpType::Atom), value(s), atomic_type(AtomicType::Symbol)
  {}

  Atom value;
  AtomicType atomic_type;

  int getInt() const
  {
    if (atomic_type == AtomicType::Int) return std::get<int>(std::get<Number>(value));
    else throw std::runtime_error("Tried to getInt from RTA that was not an Int.");
  }
  double getDouble() const
  {
    if (atomic_type == AtomicType::Double) return std::get<double>(std::get<Number>(value));
    else throw std::runtime_error("Tried to getDouble from RTA that was not a Double.");
  }
  Symbol getSymbol() const
  {
    if (atomic_type == AtomicType::Symbol) return std::get<Symbol>(value);
    else throw std::runtime_error("Tried to getSymbol from RTA that was not a Symbol.");
  }
};

std::ostream& operator<<(std::ostream& os, const RTA& x)
{
  switch (x.atomic_type) {
    case AtomicType::Int: {
      os << x.getInt();
      break;
    }
    case AtomicType::Double: {
      os << x.getDouble();
      break;
    }
    case AtomicType::Symbol: {
      os << x.getSymbol();
      break;
    }
    default: throw std::runtime_error("ostream operator<< on invalid RTA.");
  }
  return os;
}
} // namespace ryk

namespace std {
inline string to_string(const ryk::RTA& a)
{
  return boost::lexical_cast<string>(a);
}
} // namespace std

