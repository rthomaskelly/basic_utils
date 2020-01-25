#ifndef ryk_random_hpp
#define ryk_random_hpp

#include <random>

namespace ryk {

static std::random_device random_device{};
static std::mt19937 random_generator{random_device()};

template<class Iterable> inline constexpr
std::enable_if_t<is_iterable_v<Iterable>, Iterable&>
shuffle(Iterable& c)
{
  std::shuffle(c, random_generator);
}

inline constexpr double runif(double min = 0, double max = 1)
{
  return std::uniform_real_distribution<>{min, max}(random_generator);
}
inline constexpr std::vector<double> runif(uint64_t n, double min = 0, double max = 1)
{
  std::vector<double> r;
  r.reserve(n);
  for (uint64_t i = 0; i < n; ++i) r.push_back(runif(min, max)); 
  return r; 
}

inline constexpr int runif_int(int min = 0, int max = 1)
{
  return std::uniform_int_distribution<>{min, max}(random_generator);
}
inline constexpr std::vector<int> runif_int(uint64_t n, int min = 0, int max = 1)
{
  std::vector<int> r;
  r.reserve(n);
  for (uint64_t i = 0; i < n; ++i) r.push_back(runif_int(min, max)); 
  return r; 
}

} // namespace ryk

#endif
