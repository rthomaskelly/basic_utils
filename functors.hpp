#ifndef ryk_functors
#define ryk_functors

template<class T, class V>
struct plus
{
  constexpr auto operator()(const T& t, const V& v)
  {
    return t + v;
  }
};
template<class T, class V>
struct minus
{
  constexpr auto operator()(const T& t, const V& v)
  {
    return t - v;
  }
};
template<class T, class V>
struct multiplies
{
  constexpr auto operator()(const T& t, const V& v)
  {
    return t * v;
  }
};
template<class T, class V>
struct divides
{
  using result_type = decltype(operator()(declval<T&>(), declval<V&>()));
  constexpr auto operator()(const T& t, const V& v)
  {
    return t / v;
  }
};

#endif
