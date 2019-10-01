#ifndef ryk_sparse_square_matrix_hpp
#define ryk_sparse_square_matrix_hpp

#include <unordered_map>

template<class T, class H>
struct pair_hash
{
  std::size_t operator()(const std::pair<T, T>& p) const noexcept
  {
    return H(p.first) ^ H(p.second); 
  }

};

template<class T, class X, class T_Hash = std::hash<T>>
class sparse_square_matrix
{
 public:
  X& at(const T& t1, const T& t2);
  bool insert(const T& t1, const T& t2, const X& x);

 protected:
  std::unordered_map<std::pair<T, T> X, pair_hash<T, T_Hash>> the_matrix;

  std::pair<T, T> paired(const T& t1, const T& t2);
  
};

template<class T, class X>
std::pair<T, T> sparse_square_matrix::paired(const T& t1, const T& t2)
{
  auto first = T_Hash(t1) < T_Hash(t2) ? t1 : t2;
  auto second = T_Hash(t1) < T_Hash(t2) ? t2 : t1;
  return {first, second}; 
}
template<class T, class X>
X& sparse_square_matrix::at(const T& t1, const T& t2)
{
  return map.at(paired);
}
template<class T, class X>
bool insert(const T& t1, const T& t2, const X& x)
{
  return map.insert({paired, x}).second;
}

#define
