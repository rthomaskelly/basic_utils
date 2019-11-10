#ifndef ryk_functors
#define ryk_functors

namespace ryk {

template<class T, class V>
struct add
{
  using result_type = decltype(operator()(std::declval<T&>(), std::declval<V&>()));
  constexpr auto operator()(const T& t, const V& v)
  {
    return t + v;
  }
};
template<class T, class V>
struct subtract
{
  using result_type = decltype(operator()(std::declval<T&>(), std::declval<V&>()));
  constexpr auto operator()(const T& t, const V& v)
  {
    return t - v;
  }
};
template<class T, class V>
struct multiply
{
  using result_type = decltype(operator()(std::declval<T&>(), std::declval<V&>()));
  constexpr auto operator()(const T& t, const V& v)
  {
    return t * v;
  }
};
template<class T, class V>
struct divide
{
  using result_type = decltype(operator()(std::declval<T&>(), std::declval<V&>()));
  constexpr auto operator()(const T& t, const V& v)
  {
    return t / v;
  }
};

} // namespace ryk

#endif
