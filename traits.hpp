#ifndef ryk_traits_hpp
#define ryk_traits_hpp

#include <type_traits>

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
// is_pointerlike<> typetrait with is_pointerlike_v definition
//
template<class T>
decltype(++std::declval<T>(), 
         void(),
         std::declval<T>()++,
         void(*std::declval<T>()),
         std::true_type{})
pointerlike_impl(int);
template<class T> std::false_type pointerlike_impl(...);
template<class T>
using is_pointerlike = decltype(pointerlike_impl<T>(0));
template<class T> inline constexpr bool is_pointerlike_v = is_pointerlike<T>::value;

//
// is_iterator<> typetrait
//
template<class T>
struct is_iterator
{
  static constexpr bool value = is_pointerlike_v<T> && !is_iterable_v<T>;
};
template<class T> inline constexpr bool is_iterator_v = is_iterator<T>::value;


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
// has_subtype tests if a type T has a subtype of V
// this trait is needed because using subtype<T> on a non-iterable T will not compile
// it's used in the is_string<> trait below
// 
template<class T, class V>
decltype(std::integral_constant<bool, std::is_same_v<std::remove_const_t<subtype<T>>, V>>{})
has_subtype_impl(int);
template<class T, class V> std::false_type has_subtype_impl(...);
template<class T, class V>
using has_subtype = decltype(has_subtype_impl<T,V>(0));
template<class T, class V> inline constexpr bool has_subtype_v = has_subtype<T,V>::value;

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
// is_string<> with is_string_v<> definition
//
template<class T>
struct is_string
{
  static constexpr bool value = is_iterable_v<T> && has_subtype_v<T, char>;
};
template<class T> inline constexpr bool is_string_v = is_string<T>::value;

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


//
// C++-20 Concepts
//

//
// invocable
//
// (Does not compile)
// template<class F, class... Args>
// decltype(std::invoke(std::forward<F>(std::declval<F&&>()),
//                      std::forward<Args>(std::declval<Args&&...>())),
//          void(),
//          std::true_type{}) invocable_impl(int);
// template<class F, class... Args> std::false_type invocable_impl(...);
// template<class F, class... Args>
// using invocable = decltype(invocable_impl<F, Args>(0));
// template<class F, class... Args>
// inline constexpr bool invocable_v = invocable<F, Args>::value;

//
// Binary function
//
template<class F, class A, class B>
decltype(std::invoke(std::forward<F>(std::declval<F&&>()), 
                     std::forward<A>(std::declval<A&&>()),
                     std::forward<B>(std::declval<B&&>())),
         void(),
         std::true_type{}) binary_function_impl(int);
template<class F, class A, class B> std::false_type binary_function_impl(...);
template<class F, class A, class B>
using binary_function = decltype(binary_function_impl<F, A, B>(0));
template<class F, class A, class B>
inline constexpr bool binary_function_v = binary_function<F, A, B>::value;

/*
template <class From, class To>
decltype(
concept convertible_to =
  std::is_convertible_v<From, To> &&
  requires(From (&f)()) {
    static_cast<To>(f());
  };

template <class T, class U>
inline constexpr bool common_reference_with =
  std::same_as<std::common_reference_t<T, U>, std::common_reference_t<U, T>> &&
  std::convertible_to<T, std::common_reference_t<T, U>> &&
  std::convertible_to<U, std::common_reference_t<T, U>>;
  std::is_convertible_v<T, std::common_reference_t<T, U>> &&
  std::is_convertible_v<U, std::common_reference_t<T, U>>;
*/

} // namespace ryk

#endif

