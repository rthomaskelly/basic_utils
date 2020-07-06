#pragma once

#include <string>
#include <variant>
#include <boost/lexical_cast.hpp>

#include "RTN.hpp"
#include "RTTI.hpp" 


namespace ryk {

using Symbol = std::string;
using Atom = std::variant<Symbol, Number>;

class RTA : public ryk::RTTI
{
 public:
  RTA();
  RTA(int i);
  RTA(double d);
  RTA(Symbol s);

  AtomicType atomic_type() const noexcept;
  AtomicType atomicType() const noexcept;
  AtomicType getAtomicType() const noexcept;

  bool isInt() const noexcept;
  bool isDouble() const noexcept;
  bool isNumber() const noexcept;
  bool isSymbol() const noexcept;
  bool isValid() const noexcept override;

  int getInt() const;
  double getDouble() const;
  Symbol getSymbol() const;

  bool operator==(const RTA& rhs) const noexcept;
  bool operator!=(const RTA& rhs) const noexcept;

 protected:
  Atom _value;
  AtomicType _atomic_type;

  bool isEqual(const RTTI& rhs) const noexcept override;
};

inline RTA::RTA()
 : RTTI(ExpType::Atom), _value(0), _atomic_type(AtomicType::Int)
{
}
inline RTA::RTA(int i)
 : RTTI(ExpType::Atom), _value(i), _atomic_type(AtomicType::Int)
{
}
inline RTA::RTA(double d)
 : RTTI(ExpType::Atom), _value(d), _atomic_type(AtomicType::Double)
{
}
inline RTA::RTA(Symbol s)
 : RTTI(ExpType::Atom), _value(s), _atomic_type(AtomicType::Symbol)
{
}

inline AtomicType RTA::atomic_type() const noexcept
{
  return _atomic_type;
}
inline AtomicType RTA::atomicType() const noexcept
{
  return atomic_type();
}
inline AtomicType RTA::getAtomicType() const noexcept
{
  return atomic_type();
}

inline bool RTA::isInt() const noexcept
{
  return getAtomicType() == AtomicType::Int;
}
inline bool RTA::isDouble() const noexcept
{
  return getAtomicType() == AtomicType::Double;
}
inline bool RTA::isNumber() const noexcept
{
  return isInt() || isDouble();
}
inline bool RTA::isSymbol() const noexcept
{
  return getAtomicType() == AtomicType::Symbol;
} 
inline bool RTA::isValid() const noexcept
{
  return isNumber() || isSymbol();
}

inline int RTA::getInt() const
{
  if (isInt()) return std::get<int>(std::get<Number>(_value));
  else throw std::runtime_error("Tried to getInt from RTA that was not an Int.");
}
inline double RTA::getDouble() const
{
  if (isDouble()) return std::get<double>(std::get<Number>(_value));
  else throw std::runtime_error("Tried to getDouble from RTA that was not a Double.");
}
inline Symbol RTA::getSymbol() const
{
  if (isSymbol()) return std::get<Symbol>(_value);
  else throw std::runtime_error("Tried to getSymbol from RTA that was not a Symbol.");
}

inline bool RTA::operator==(const RTA& rhs) const noexcept
{
  if (atomicType() != rhs.atomicType() || !isValid()) return false;
  if (isInt()) return getInt() == rhs.getInt();
  if (isDouble()) return getDouble() == rhs.getDouble();
  if (isSymbol()) return getSymbol() == rhs.getSymbol();
  return false; // catchall
}
inline bool RTA::operator!=(const RTA& rhs) const noexcept
{
  return !(*this == rhs);
}
inline bool RTA::isEqual(const RTTI& rhs) const noexcept
{
  return *this == dynamic_cast<const RTA&>(rhs);
}

std::ostream& operator<<(std::ostream& os, const RTA& x)
{
  switch (x.atomicType()) {
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
    default: throw std::runtime_error("ostream operator<< on invalid RTA."
                " Has atomicType value of '" + std::to_string(x.atomicType()) + "'.");
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

