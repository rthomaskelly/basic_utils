#ifndef ryk_sparse_square_matrix_hpp
#define ryk_sparse_square_matrix_hpp

#include <unordered_map>


namespace ryk {

template<class T, class H>
struct pair_hash
{
  std::size_t operator()(const std::pair<T, T>& p) const noexcept
  {
    return H{}(p.first) ^ H{}(p.second); 
  }

};

template<class T, class X, class T_Hash = std::hash<T>>
class sparse_square_matrix
{
 public:
  X& at(const T& t1, const T& t2);
  bool insert(const T& t1, const T& t2, const X& x);
 
  template<class V, class Y, class H>
  friend std::ostream& operator<<(std::ostream&, const sparse_square_matrix<V, Y, H>&);

 protected:
  std::unordered_map<std::pair<T, T>, X, pair_hash<T, T_Hash>> the_matrix;

  std::pair<T, T> paired(const T& t1, const T& t2);
  
};

template<class T, class X, class T_Hash>
std::pair<T, T> sparse_square_matrix<T, X, T_Hash>::paired(const T& t1, const T& t2)
{
  auto first = T_Hash{}(t1) < T_Hash{}(t2) ? t1 : t2;
  auto second = T_Hash{}(t1) < T_Hash{}(t2) ? t2 : t1;
  return {first, second}; 
}
template<class T, class X, class T_Hash>
X& sparse_square_matrix<T, X, T_Hash>::at(const T& t1, const T& t2)
{
  return the_matrix.at(paired(t1, t2));
}
template<class T, class X, class T_Hash>
bool sparse_square_matrix<T, X, T_Hash>::insert(const T& t1, const T& t2, const X& x)
{
  return the_matrix.insert({paired(t1, t2), x}).second;
}

template<class T, class X, class H>
std::ostream& operator<<(std::ostream& os, const sparse_square_matrix<T, X, H>& ssm)
{
  for (auto& p : ssm.the_matrix)
    os << "[" << p.first.first << ", " << p.first.second << "]=" << p.second << "\n";
  return os;
}

} // namespace ryk
#endif
