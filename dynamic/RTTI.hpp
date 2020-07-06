#pragma once

#include <string>
#include <boost/lexical_cast.hpp>
#include "runtime_enums.hpp"

namespace ryk {

//
// Interface to allow getting Runtime type info
class RTTI
{
 public:
  RTTI();
  RTTI(ExpType newtype);
  virtual ~RTTI() = default;

  ExpType type() const noexcept;

  virtual bool isAtom() const noexcept;
  virtual bool isList() const noexcept;
  bool isExp() const noexcept;

  virtual bool isValid() const noexcept;
  bool checkValid() const;

  bool operator==(const RTTI& rhs) const noexcept;

 protected:
  ExpType _type;

  virtual bool isEqual(const RTTI& rhs) const noexcept = 0;
};

inline RTTI::RTTI()
{
}
inline RTTI::RTTI(ExpType newtype)
 : _type(newtype)
{
}
inline ExpType RTTI::type() const noexcept
{
  return _type;
}
inline bool RTTI::isAtom() const noexcept
{
  return type() == ExpType::Atom;
}
inline bool RTTI::isList() const noexcept
{
  return type() == ExpType::List;
}
inline bool RTTI::isExp() const noexcept
{
  return type() == ExpType::Exp;
}
inline bool RTTI::isValid() const noexcept
{
  return type() == ExpType::Exp || type() == ExpType::Atom || type() == ExpType::List;
}
inline bool RTTI::checkValid() const
{
  if (isValid()) return true;
  else throw std::runtime_error(std::to_string(type()) + " Found in checkValid().");
}
inline bool RTTI::operator==(const RTTI& rhs) const noexcept
{
  return type() == rhs.type() && isEqual(rhs); 
}

} // namespace ryk
