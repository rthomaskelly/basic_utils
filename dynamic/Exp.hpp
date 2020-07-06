#pragma once

#include <string>
#include <variant>
#include <boost/lexical_cast.hpp>

#include <basic_utils/dynamic/runtime_list.hpp>
#include "RTA.hpp"

namespace ryk {

using List = ryk::runtime_list;
using ExpT = std::variant<RTA, List>;

struct Exp : public RTTI
{
  Exp();
  Exp(RTA a);
  Exp(List l);

  const RTA& getAtom() const;
  RTA& getAtom();
  const List& getList() const;
  List& getList();

  ExpType subtype() const noexcept;
  bool isAtom() const noexcept override;
  bool isList() const noexcept override;
  bool isValid() const noexcept override;
  bool isSymbol() const;
  bool isInt() const;
  bool isDouble() const;

  int getInt() const;
  double getDouble() const;
  Symbol getSymbol() const;
  std::size_t size() const;

  bool operator==(const Exp& rhs) const noexcept;
  bool operator!=(const Exp& rhs) const noexcept;

 protected:
  ExpType _subtype;
  ExpT _value;

  bool isEqual(const RTTI& rhs) const noexcept override;
};

inline Exp::Exp()
 : RTTI(ExpType::Exp), _subtype(ExpType::Atom), _value{RTA{}}
{
}
inline Exp::Exp(RTA a)
 : RTTI(ExpType::Exp), _subtype(ExpType::Atom), _value(a)
{
}
inline Exp::Exp(List l)
 : RTTI(ExpType::Exp), _subtype(ExpType::List), _value(l)
{
}

inline const RTA& Exp::getAtom() const
{
  if (isAtom()) return std::get<RTA>(_value);
  else throw std::runtime_error("Tried to getAtom from Exp that was not an Atom.");
}
inline RTA& Exp::getAtom()
{
  return const_cast<RTA&>(const_cast<const Exp*>(this)->getAtom());
}
inline const List& Exp::getList() const
{
  if (isList()) return std::get<List>(_value);
  else throw std::runtime_error("Tried to getList from Exp that was not a List.");
}
inline List& Exp::getList()
{
  return const_cast<List&>(const_cast<const Exp*>(this)->getList());
}

inline ExpType Exp::subtype() const noexcept
{
  return _subtype;
}
inline bool Exp::isAtom() const noexcept
{
  return subtype() == ExpType::Atom;
}
inline bool Exp::isList() const noexcept
{
  return subtype() == ExpType::List;
}
inline bool Exp::isValid() const noexcept
{
  return type() == ExpType::Exp && (isAtom() || isList());
}
inline bool Exp::isSymbol() const
{
  return isAtom() && getAtom().isSymbol();
}
inline bool Exp::isInt() const
{
  return isAtom() && getAtom().isInt();
}
inline bool Exp::isDouble() const
{
  return isAtom() && getAtom().isDouble();
}

inline int Exp::getInt() const
{
  if (isAtom()) return getAtom().getInt();
  else throw std::runtime_error("Tried to getInt from an Exp that was not an Atom.");
}
inline double Exp::getDouble() const
{
  if (isDouble()) return getAtom().getDouble(); 
  else throw std::runtime_error("Tried to getDouble from an Exp that was not an Atom.");
}
inline Symbol Exp::getSymbol() const
{
  if (isSymbol()) return getAtom().getSymbol();
  else throw std::runtime_error("Tried to getSymbol from an Exp that was not an Atom.");
}
inline std::size_t Exp::size() const
{
  checkValid();
  if (isAtom()) return 1;
  else return getList().size();  
}

inline bool Exp::operator==(const Exp& rhs) const noexcept
{
  if (type() != rhs.type() || !isValid()) return false;
  if (isAtom()) return getAtom() == rhs.getAtom(); 
  if (isList()) return getList() == rhs.getList();
  return false;
}
inline bool Exp::operator!=(const Exp& rhs) const noexcept
{
  return !(*this == rhs);
}
inline bool Exp::isEqual(const RTTI& rhs) const noexcept
{
  return *this == dynamic_cast<const Exp&>(rhs);
}

} // namespace ryk


