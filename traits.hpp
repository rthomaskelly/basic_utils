#ifndef ryk_traits_hpp
#define ryk_traits_hpp

namespace ryk {

//
// enable_if_p is used in template paramters to enable functions
// like so:
// template<class T, enable_if_p<std::is_integral_v<T>>...>
// note the '...' parameter-pack ending, it won't work without it!
// see: https://stackoverflow.com/questions/20709896/how-do-i-use-stdenable-if-with-a-self-deducing-return-type
//
template<bool b>
using enable_if_p = std::enable_if_t<b, void>;

//
// is_iterable<> typetrait with is_iterable_v and iterable(T) definitions
//
template<class T>
decltype(std::declval<T&>().begin() != std::declval<T&>().end(),
	 void(),
	 ++std::declval<decltype(std::begin(std::declval<T&>()))&>(),
	 void(*std::begin(std::declval<T&>())),
	 std::true_type{})
iterable_impl(int);
template<class T> std::false_type iterable_impl(...);
template<class T>
using is_iterable = decltype(iterable_impl<T>(0));
template<class T> inline constexpr bool is_iterable_v = is_iterable<T>::value;
template<class T> inline constexpr
bool iterable(T t)
{
  return is_iterable_v<T>;
}

//
// iterator<> type and subtype<> of an iterable
// also deref<> type
//
template<class T>
using iterator = decltype(std::declval<T&>().begin());

template<class T>
using deref = decltype(*std::declval<T>());

template<class Container>
using subtype = std::remove_reference_t<deref<iterator<Container>>>;
template<class Container>
using elementtype = subtype<Container>;
template<class Container>
using itemtype = subtype<Container>;

// 
// is_indexable<> with is_indexable_v<> and indexable(T) definitions
//
template<class T>
decltype(std::declval<T&>()[0], std::true_type{}) is_indexable_impl(int);
template<class T> std::false_type is_indexable_impl(...);
template<class T>
using is_indexable = decltype(is_indexable_impl<T>(0));
template<class T> inline constexpr bool is_indexable_v = is_indexable<T>::value;
template<class T> inline constexpr
bool indexable(T t)
{
  return is_indexable_v<T>;
}

//
// has_map_types<> and is_map_v<> with map(T) definition to detect maps
//
template<class T>
decltype(std::declval<typename T::key_type&>(),
         std::declval<typename T::mapped_type&>(), 
         std::declval<typename T::value_type&>(), 
         std::true_type{}) has_map_types_impl(int);
template<class T> std::false_type has_map_types_impl(...);
template<class T>
using has_map_types = decltype(has_map_types_impl<T>(0));
template<class T> inline constexpr bool has_map_types_v = has_map_types<T>::value;
template<class T> 
struct is_map
{ 
  static constexpr bool value = is_iterable_v<T> && has_map_types_v<T>; 
};
template<class T> inline constexpr bool is_map_v = is_map<T>::value;
template<class T> inline
constexpr bool map(T t)
{
  return is_map_v<T>;
}

//
// distance type
//
template<class T>
using distance = decltype(std::declval<T&>().end() - std::declval<T&>().begin());

//
// insertable
//
template<class T>
decltype(std::declval<T&>().insert(std::declval<iterator<T>>(), 
                                   std::declval<const subtype<T>&>()),
         void(),
         std::true_type{}) insertable_impl(int);
template<class T> std::false_type insertable_impl(...);
template<class T>
using is_insertable = decltype(insertable_impl<T>(0));
template<class T> inline constexpr bool is_insertable_v = is_insertable<T>::value;

} // namespace ryk

#endif

