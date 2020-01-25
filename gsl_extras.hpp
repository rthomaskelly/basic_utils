#ifndef ryk_gsl_extras_hpp
#define ryk_gsl_extras_hpp

#include <gsl/gsl>
#include "traits.hpp"

namespace gsl {

template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, span<deref<Iterator>>>
make_span(Iterator iter, typename span<deref<Iterator>>::index_type count)
{
  return span<deref<Iterator>>(iter, count);
}
template<class Iterator> inline constexpr
std::enable_if_t<is_iterator_v<Iterator>, span<deref<Iterator>>>
make_span(Iterator first, Iterator last)
{
  return span<deref<Iterator>>(first, last);
}

} // namespace gsl

#endif
