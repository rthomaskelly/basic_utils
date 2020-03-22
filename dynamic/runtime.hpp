
#include <string>
#include <boost/lexical_cast.hpp>

namespace ryk {

// Run time type tags for the basic types
enum class AtomicType
{
  Symbol, Int, Double, List 
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
      os << "Unknown AtomicType.";
    }
  }
  return os;
}

//
// Run time type tags for an Exp (Expression)
// Expressions are composed of Atoms and Lists
enum class ExpType : char
{ 
  Atom = 'A', List = 'L'
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
    default: {
      os << "Unknown ExpType.";
    }
  }
  return os;
}

//
// Interface to allow getting Runtime type info
class RTTI
{
 public:
  RTTI() {}
  RTTI(ExpType newtype) : _type(newtype) {}
  virtual ~RTTI() = default;

  ExpType type() const { return _type; }

 protected:
  ExpType _type;
};



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
