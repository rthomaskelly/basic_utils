#ifndef ryk_boxed_types
#define ryk_boxed_types

#include <stdint.h>

namespace ryk {

//
// base class to create boxed types
//
template<class T>
class boxed_type
{
 public:
  constexpr boxed_type();
  constexpr boxed_type(const T tt);
  
  // I don't have a great name for the actual type that is boxed
  // inside a boxedtype
  // an obvious name is just boxed_type but we can't have an alias
  // the same name as the class type
  // 'type' seems like a good option but that doesn't allow a boxed_type to 
  // be testable (i.e. how then to implement is_boxed_type?)
  using type = T;
  // using boxed_type = T;
  using boxedtype = T;
  using the_boxed_type = T;
  using the_boxedtype = T;
  
  constexpr operator T() const;
 protected:
  T t; 
};

template<class T> inline constexpr
boxed_type<T>::boxed_type()
{
}

template<class T> inline constexpr
boxed_type<T>::boxed_type(const T tt) : t(tt) 
{
}

template<class T> inline constexpr
boxed_type<T>::operator T() const
{
  return t;
}

template<class T> using boxedtype = boxed_type<T>;

template<class T>
struct boxed_hash
{
  constexpr std::size_t operator()(T t) const noexcept;
};

template<class T> inline constexpr
std::size_t boxed_hash<T>::operator()(T t) const noexcept 
{
  return std::hash<typename T::boxedtype>{}(static_cast<typename T::boxedtype>(t));
}

//
// is_boxed_type<T> type trait with is_boxed_type_v<T> definitions
//
template<class T>
decltype(std::declval<typename T::boxed_type&>(), std::true_type{})
has_boxed_type_impl(int);
template<class T> std::false_type has_boxed_type_impl(...);
template<class T>
using has_boxed_type = decltype(has_boxed_type_impl<T>(0));

template<class T>
decltype(std::declval<typename T::boxedtype&>(), std::true_type{})
has_boxedtype_impl(int);
template<class T> std::false_type has_boxedtype_impl(...);
template<class T>
using has_boxedtype = decltype(has_boxedtype_impl<T>(0));

template<class T>
struct is_boxed_type
{
  static constexpr bool value = has_boxed_type<T>::value || has_boxedtype<T>::value;
};
template<class T> inline constexpr bool is_boxed_type_v = is_boxed_type<T>::value;

//
// is_scalar<T> and is_arithmetic<T> definitions including boxed_types
//
template<class T> struct is_scalar 
{
  static constexpr bool value = std::is_scalar<T>::value 
    || std::is_scalar<typename T::boxedtype>::value;
};
template<class T> inline constexpr bool is_scalar_v = is_scalar<T>::value;

template<class T> struct is_arithmetic
{
  static constexpr bool value = std::is_arithmetic<T>::value
    || std::is_arithmetic<typename T::boxedtype>::value;
}; 
template<class T> inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

//
// specializations creating the basic boxed types
//
class Int : public boxed_type<int>
{
 public:
  constexpr Int();
  constexpr Int(const int tt);
};

inline constexpr Int::Int()
{
}

inline constexpr Int::Int(const int tt) : boxed_type(tt)
{
}

class Int64 : public boxed_type<int64_t>
{
 public:
  constexpr Int64();
  constexpr Int64(const int64_t tt);
};

inline constexpr Int64::Int64()
{
}

inline constexpr Int64::Int64(const int64_t tt) : boxed_type(tt)
{
}

using Int64_t = Int64;

class UInt : public boxed_type<unsigned int>
{
 public:
  constexpr UInt();
  constexpr UInt(const unsigned int tt);
};

inline constexpr UInt::UInt()
{
}

inline constexpr UInt::UInt(const unsigned int tt) : boxed_type(tt)
{
}

class UInt64 : public boxed_type<uint64_t>
{
 public:
  constexpr UInt64();
  constexpr UInt64(const uint64_t tt);
};

inline constexpr UInt64::UInt64()
{
}

inline constexpr UInt64::UInt64(const uint64_t tt) : boxed_type(tt)
{
}

using UInt64_t = UInt64;
using Nat = UInt64;

class Double : public boxed_type<double>
{
 public:
  constexpr Double();
  constexpr Double(const double tt);
};

inline constexpr Double::Double()
{
}

inline constexpr Double::Double(const double tt) : boxed_type(tt)
{
}

class Size_t : public boxed_type<std::size_t>
{
 public:
  constexpr Size_t();
  constexpr Size_t(const std::size_t tt);
};

inline constexpr Size_t::Size_t()
{
}

inline constexpr Size_t::Size_t(const std::size_t tt) : boxed_type(tt)
{
}

} // namespace ryk

#endif
