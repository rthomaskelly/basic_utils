#pragma once

#include <string>
#include <variant>
#include <boost/lexical_cast.hpp>

namespace ryk {

using Number = std::variant<int, double>;

class RTN
{
 public:
  template<class T>
  RTN(T new_value, AtomicType new_type)
    : value(new_value), type(new_type)
  {}

  Number value;
  AtomicType type; 

  int getInt() const
  {
    if (type == AtomicType::Int) return std::get<int>(value);
    else throw std::runtime_error("Tried to getInt from RTN that was not an Int.");
  }
  double getDouble() const
  {
    if (type == AtomicType::Double) return std::get<double>(value);
    else throw std::runtime_error("Tried to getDouble from RTN that was not a Double.");
  }
};

std::ostream& operator<<(std::ostream& os, const RTN& x)
{
  switch (x.type) {
    case AtomicType::Int: {
      os << x.getInt();
      break;
    }
    case AtomicType::Double: {
      os << x.getDouble();
      break;
    }
    default: throw std::runtime_error("Tried to print RTN that was neither Int or Double.");
  }
  return os;
}
} // namespace ryk

namespace std {
inline string to_string(const ryk::RTN& n)
{
  return boost::lexical_cast<string>(n);
}
} // namespace std


