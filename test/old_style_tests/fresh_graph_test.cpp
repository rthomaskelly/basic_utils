#include <iostream>
#include <cstddef>

#include "graph_ll_fresh.hpp"

using namespace ryk;
using std::cout;
using std::endl;

template<class T, class U> constexpr size_t offset_of(U T::*member)
{
    return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}


template<class N, class E>
using GN = typename fresh_graph<N, E>::graph_node;

template<class N, class E>
N* GNptr_to_dataptr(GN<N,E>* nptr)
{
  using GNNE = GN<N,E>;
  char offset = offsetof(GNNE, data);
  return nullptr; 
}

template<class N, class E>
GN<N, E>* dataptr_to_GNptr(N* nptr, E)
{
  using GNNE = GN<N,E>;
  char offset = offsetof(GNNE, data);
  char* nptr_bytes = reinterpret_cast<char*>(nptr);
  nptr_bytes -= offset;
  return reinterpret_cast<GNNE*>(nptr_bytes);
}

int main(int argc, char** argv)
{
  fresh_graph<int, int> g(0);
  
  g.add_child(0, 1);
  using node = typename fresh_graph<int, int>::graph_node;

  // for (auto shp : g.root_nodes())
  // {
  //  cout << &shp->data << " @ " << shp.get() << endl;
  //  cout << (char*)&shp->data - (char*)shp.get() << endl;
  //  cout << (char*)&shp->data - offset_of(&node::data) << endl;
  // }
  // cout << offset_of(&fresh_graph<int, int>::graph_node::data) << " " << offsetof(node, data);
  cout << endl;
  
  // cout << g << endl;
  
  return 0;
}
