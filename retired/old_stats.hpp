//
// common and statistical functions
// TODO: median, quartiles, percentiles
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
sum(const Iterable& c, const subtype<Iterable>& init = subtype<Iterable>{})
{
  return accumulate(c, 
    [](const subtype<Iterable>& t1, const subtype<Iterable>& t2) { return t1 + t2; });
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
product(const Iterable& c, const subtype<Iterable>& init = subtype<Iterable>{})
{
  return accumulate(c, 
    [](const subtype<Iterable>& t1, const subtype<Iterable>& t2) { return t1 * t2; });
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
mean(const Iterable& c)
{
  if (c.size() != 0) return sum(c) / c.size();
  return subtype<Iterable>{};
}

//
// 'centered_squares' - the (x_i - mu)^2 component used in calculating variance
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
affect_centered_squares(Iterable& c, const subtype<Iterable>& meanval)
{
  return affect(c, [meanval](subtype<Iterable> t) { return (t - meanval) * (t - meanval); });
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
affect_centered_squares(Iterable& c)
{
  return affect_centered_squares(c, mean(c));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, Iterable>
centered_squares(Iterable c, const subtype<Iterable>& meanval)
{
  return affect_centered_squares(c, meanval); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, Iterable>
centered_squares(const Iterable& c)
{
  return centered_squares(c, mean(c));
}

//
// variance and vol (vol is standard deviation)
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_variance(Iterable& c, const subtype<Iterable>& meanval)
{
  return mean(affect_centered_squares(c, meanval));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_variance(Iterable& c)
{
  return affect_variance(c, mean(c));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
variance(Iterable c, const subtype<Iterable>& meanval)
{
  return affect_variance(c, meanval);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
variance(const Iterable& c)
{
  return variance(c, mean(c));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
affect_vol(Iterable& c, const subtype<Iterable>& meanval)
{
  return sqrt(affect_variance(c, meanval));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
affect_vol(Iterable& c)
{
  return affect_vol(c, mean(c)); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
vol(Iterable c, const subtype<Iterable>& meanval)
{
  return affect_vol(c, meanval); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable<Iterable>::value, subtype<Iterable>>
vol(const Iterable& c)
{
  return vol(c, mean(c));
}

//
// median
//
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_median(Iterable& c)
{
  if (c.size() != 0) {
    sort(c);
    if (!is_even(c.size())) return at(c, c.size() / 2);
    else return (at(c, c.size() / 2) + at(c, c.size() / 2 + 1)) / 2; 
  }
  else return 0;
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
median(Iterable c)
{
  return affect_median(c);
}
