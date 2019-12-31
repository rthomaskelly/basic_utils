#include <iostream>
#include <cstddef>

#include "graph_deferred.hpp"

using namespace ryk;
using namespace gcpp;
using std::cout;
using std::endl;

template<class T>
struct deferred_hash
{
  std::size_t operator()(const gcpp::deferred_ptr<T>& p) const noexcept
  {
    return std::hash<const T*>(p.get());
  }
};

template<class T>
struct d_hash;

template<class T>
struct d_hash<gcpp::deferred_ptr<T>>
{
  std::size_t operator()(const gcpp::deferred_ptr<T>& p) const noexcept
  {
    return std::hash<typename gcpp::deferred_ptr<T>::element_type*>()(p.get());
  }
};

int main(int argc, char** argv)
{
  deferred_graph<int, int> g(0);
  auto& roots = g.root_nodes();
  deferred_ptr<deferred_graph<int, int>::graph_node> root = roots.at(0); 
  g.add_child(root, 1);
  
  auto ptr = the_graph_heap.make<deferred_graph<int, int>::graph_node>(10);
  
  // deferred_unordered_map<decltype(ptr), int, deferred_hash<node_type>> m;
  cout << g << endl;
  //using node = typename fresh_graph<int, int>::graph_node;

  // for (auto shp : g.root_nodes())
  // {
  //  cout << &shp->data << " @ " << shp.get() << endl;
  //  cout << (char*)&shp->data - (char*)shp.get() << endl;
  //  cout << (char*)&shp->data - offset_of(&node::data) << endl;
  // }
  // cout << offset_of(&fresh_graph<int, int>::graph_node::data) << " " << offsetof(node, data);
  
  
  return 0;
}
