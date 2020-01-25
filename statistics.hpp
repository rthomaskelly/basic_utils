#ifndef ryk_statistics_hpp
#define ryk_statistics_hpp

#include "iterable_algorithms.hpp"

namespace ryk {

//
// sum - synonymous with accumulate's default argument version
//
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
sum(Iterator first, Iterator last)
{
  return ryk::accumulate(first, last);
} 
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
sum(Iterator first, Iterator last, deref<Iterator> init)
{
  return std::accumulate(first, last, init);
} 
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
sum(const Iterable& c)
{
  return sum(c.begin(), c.end());
}

//
// product
//
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
product(Iterator first, Iterator last)
{
  return ryk::accumulate(first, last, [](auto a, auto b){ return a * b; });
} 
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
product(Iterator first, Iterator last, deref<Iterator> init)
{
  return std::accumulate(first, last, init, [](auto a, auto b){ return a * b; });
} 
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
product(const Iterable& c)
{
  return product(c.begin(), c.end());
}

//
// mean with average synonym 
//
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
mean(Iterator first, Iterator last)
{
  auto size = std::distance(first, last);
  if (size > 0) return sum(first, last) / size;
  else return std::decay_t<deref<Iterator>>{};
} 
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
mean(const Iterable& c)
{
  return mean(c.begin(), c.end());
}
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
average(Iterator first, Iterator last)
{
  return mean(first, last);
} 
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
average(const Iterable& c)
{
  return mean(c);
}

//
// centered squares (the (x_i - mu)^2 component used in calculating variance)
//
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
affect_centered_squares(Iterator first, Iterator last, deref<Iterator> the_mean)
{
  return std::transform(first, last, first, 
                        [the_mean](auto x){ return (x - the_mean) * (x - the_mean); });
}
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
affect_centered_squares(Iterator first, Iterator last)
{
  return affect_centered_squares(first, last, mean(first, last));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
affect_centered_squares(Iterable& c, subtype<Iterable> the_mean)
{
  affect_centered_squares(c.begin(), c.end(), the_mean);
  return c;
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
affect_centered_squares(Iterable& c)
{
  return affect_centered_squares(c, mean(c));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable>
centered_squares(Iterable c, subtype<Iterable> the_mean)
{
  return affect_centered_squares(c, the_mean); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable>
centered_squares(const Iterable& c)
{
  return centered_squares(c, mean(c));
}

//
// variance
//
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
affect_variance(Iterator first, Iterator last, deref<Iterator> the_mean)
{
  return mean(affect_centered_squares(first, last, the_mean));
}
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
affect_variance(Iterator first, Iterator last)
{
  return affect_variance(first, last, mean(first, last)); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_variance(Iterable& c, subtype<Iterable> the_mean)
{
  return affect_variance(c.begin(), c.end(), the_mean); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_variance(Iterable& c)
{
  return affect_variance(c, mean(c));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
variance(Iterable c, subtype<Iterable> the_mean)
{
  return affect_variance(c, the_mean);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
variance(const Iterable& c)
{
  return variance(c, mean(c));
}

//
// stdev is standard deviation
//
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
affect_stdev(Iterator first, Iterator last, deref<Iterator> the_mean)
{
  return sqrt(affect_variance(first, last, the_mean));
}
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, std::decay_t<deref<Iterator>>>
affect_stdev(Iterator first, Iterator last)
{
  return affect_stdev(first, last, mean(first, last)); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_stdev(Iterable& c, subtype<Iterable> the_mean)
{
  return affect_stdev(c.begin(), c.end(), the_mean); 
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_stdev(Iterable& c)
{
  return affect_stdev(c, mean(c));
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
stdev(Iterable c, subtype<Iterable> the_mean)
{
  return affect_stdev(c, the_mean);
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
stdev(const Iterable& c)
{
  return stdev(c, mean(c));
}

//
// median
//
template<class SortedIterator> inline constexpr
std::enable_if_t<is_iterator_v<SortedIterator>, std::decay_t<deref<SortedIterator>>>
median(SortedIterator first, SortedIterator last)
{
  auto size = std::distance(first, last);
  if (size > 0) {
    if (!is_even(size)) {
      std::advance(first, size / 2);
      return *first;
    }
    else {
      std::advance(first, size / 2 - 1);
      return (*first + *std::next(first)) / 2; 
    }
  }
  else return std::decay_t<deref<SortedIterator>>{};
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
affect_median(Iterable& c)
{
  ryk::sort(c);
  return median(c.begin(), c.end());
}
template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, subtype<Iterable>>
median(Iterable c)
{
  return affect_median(c);
}

//
// percentiles - takes a range of data, a range of percentiles (0 to 100) and an output iterator
// the last two signatures give a vector of results
//
template<class DataIterator, class PercIterator, class OutputIterator> inline constexpr
std::enable_if_t<is_iterator_v<DataIterator> && is_iterator_v<PercIterator>
                 && is_iterator_v<OutputIterator>, OutputIterator>
percentiles(DataIterator d_first, DataIterator d_last, PercIterator p_first, 
            PercIterator p_last, OutputIterator out)
{
  auto size = std::distance(d_first, d_last);
  if (size > 0) {
    int last_index(0); 
    for (; p_first != p_last; ++p_first) {
      uint64_t index = static_cast<double>(*p_first) / 100 * (size - 1);
      std::advance(d_first, index - last_index);
      *out++ = *d_first;
      last_index = index;
    }
  } else {
    for (; p_first != p_last; ++p_first) *out++ = std::decay_t<deref<DataIterator>>{};
  }
  return out;
}
template<class DataIterable, class PercIterable, class OutputIterator> inline constexpr
std::enable_if_t<is_iterable_v<DataIterable> && is_iterable_v<PercIterable>
                 && is_iterator_v<OutputIterator>, OutputIterator>
affect_percentiles(DataIterable& data, const PercIterable& the_percentiles, OutputIterator out)
{
  ryk::sort(data);
  return percentiles(data.begin(), data.end(), 
                     the_percentiles.begin(), the_percentiles.end(), out);
}
template<class DataIterable, class PercIterable, class OutputIterator> inline constexpr
std::enable_if_t<is_iterable_v<DataIterable> && is_iterable_v<PercIterable>
                 && is_iterator_v<OutputIterator>, OutputIterator>
percentiles(DataIterable data, const PercIterable& the_percentiles, OutputIterator out)
{
  return affect_percentiles(data.begin(), data.end(), 
                            the_percentiles.begin(), the_percentiles.end(), out);
}
template<class DataIterable, class PercIterable> inline constexpr
std::enable_if_t<is_iterable_v<DataIterable> && is_iterable_v<PercIterable>
                 && !is_map_v<PercIterable>, std::vector<subtype<DataIterable>>>
affect_percentiles(DataIterable& data, const PercIterable& the_percentiles) 
{
  std::vector<subtype<DataIterable>> r; 
  r.reserve(the_percentiles.size());
  ryk::sort(data);
  percentiles(data.begin(), data.end(), 
              the_percentiles.begin(), the_percentiles.end(), r.begin());
  return r;
}
template<class DataIterable, class PercIterable> inline constexpr
std::enable_if_t<is_iterable_v<DataIterable> && is_iterable_v<PercIterable>
                 && !is_map_v<PercIterable>, std::vector<subtype<DataIterable>>>
percentiles(DataIterable data, const PercIterable& the_percentiles)
{
  return percentiles(data, the_percentiles);
}
//
// percentiles now for map inputs
//
template<class DataIterator, class PercIterator> inline constexpr
std::enable_if_t<is_iterator_v<DataIterator> && is_pair_iterator_v<PercIterator>, PercIterator>
percentiles(DataIterator d_first, DataIterator d_last, 
            PercIterator p_first, PercIterator p_last) 
{
  auto size = std::distance(d_first, d_last);
  if (size > 0) {
    int last_index(0); 
    for (; p_first != p_last; ++p_first) {
      uint64_t index = static_cast<double>(p_first->first) / 100 * (size - 1);
      std::advance(d_first, index - last_index);
      p_first->second += *d_first;
      last_index = index;
    }
  } else {
    for (; p_first != p_last; ++p_first) p_first->second = std::decay_t<deref<DataIterator>>{};
  }
  return p_first;
}
template<class DataIterable, class PercMap> inline constexpr
std::enable_if_t<is_iterable_v<DataIterable> && is_map_v<PercMap>, PercMap&>
affect_percentiles(DataIterable& data, PercMap& percentiles_map) 
{
  ryk::sort(data);
  percentiles(data.begin(), data.end(), percentiles_map.begin(), percentiles_map.end());
  return percentiles_map;
}
template<class DataIterable, class PercMap> inline constexpr
std::enable_if_t<is_iterable_v<DataIterable> && is_map_v<PercMap>, PercMap&>
percentiles(DataIterable data, PercMap& percentiles_map) 
{
  return affect_percentiles(data, percentiles_map);
}

//
// quartiles 
//
template<class DataIterable> inline constexpr
std::enable_if_t<is_iterable_v<DataIterable>, std::map<int, subtype<DataIterable>>>
affect_quartiles(DataIterable& c)
{
  auto d = subtype<DataIterable>{};
  std::map<int, subtype<DataIterable>> r{
    {0, d}, {25, d}, {50, d}, {75, d}, {100, d}
  };
  return affect_percentiles(c, r);
}
template<class DataIterable> inline constexpr
std::enable_if_t<is_iterable_v<DataIterable>, std::map<int, subtype<DataIterable>>>
quartiles(DataIterable c)
{
  return affect_quartiles(c); 
}

} // namespace ryk
#endif
