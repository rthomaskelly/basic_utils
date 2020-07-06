#pragma once

#include <string>
#include <boost/lexical_cast.hpp>

namespace ryk {

// Run time type tags for the basic types
enum class AtomicType : char
{
  Symbol = 'S', Int = 'I', Double = 'D', List = 'L' 
};

std::ostream& operator<<(std::ostream& os, AtomicType t)
{
  switch (t) {
    case AtomicType::Symbol: {
      os << "Symbol";
      break;
    }
    case AtomicType::Int: {
      os << "Int";
      break;
    }
    case AtomicType::Double: {
      os << "Double";
      break;
    }
    case AtomicType::List: {
      os << "List";
      break;
    }
    default: {
      os << "Unknown AtomicType '" << static_cast<char>(t) << "'.";
    }
  }
  return os;
}

//
// Run time type tags for an Exp (Expression)
// Expressions are composed of Atoms and Lists
enum class ExpType : char
{ 
  Atom = 'A', List = 'L', Exp = 'E'
};

inline std::ostream& operator<<(std::ostream& os, ExpType t)
{
  switch (t) {
    case ExpType::Atom: {
      os << "Atom";
      break;
    }
    case ExpType::List: {
      os << "List";
      break;
    }
    case ExpType::Exp: {
      os << "Exp";
      break;
    }
    default: {
      os << "Unknown ExpType '" << static_cast<char>(t) << "'.";
    }
  }
  return os;
}

} // namespace ryk

namespace std {
inline string to_string(ryk::AtomicType t)
{
  return boost::lexical_cast<string>(t);   
}

inline string to_string(ryk::ExpType t)
{
  return boost::lexical_cast<string>(t);   
}
} // namespace std
