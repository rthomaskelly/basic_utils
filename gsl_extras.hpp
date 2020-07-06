#ifndef ryk_gsl_extras_hpp
#define ryk_gsl_extras_hpp

#include <gsl/gsl>
#include "traits.hpp"

namespace gsl {

template<class Iterator> inline constexpr
std::enable_if_t<ryk::is_iterator_v<Iterator>, span<ryk::deref<Iterator>>>
make_span(Iterator iter, typename span<ryk::deref<Iterator>>::index_type count)
{
  return span<ryk::deref<Iterator>>(iter, count);
}
template<class Iterator> inline constexpr
std::enable_if_t<ryk::is_iterator_v<Iterator>, span<ryk::deref<Iterator>>>
make_span(Iterator first, Iterator last)
{
  return span<ryk::deref<Iterator>>(first, last);
}

} // namespace gsl

#endif
