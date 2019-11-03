#ifndef ryk_fresh_graph
#define ryk_fresh_graph

#include <memory>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "iterable_algorithms.hpp"
#include "algorithm_extras.hpp"

namespace ryk {


template<class Node, class Edge>
class fresh_graph
{
 public:
  struct graph_node
  {
    graph_node(const Node& new_data)
     : data(new_data)
    {
    }
    std::vector<std::pair<std::shared_ptr<graph_node>, Edge>> children;
    std::vector<std::pair<graph_node*, Edge>> parents;
  
    Node data;
    
          
  };

  fresh_graph();
  
  fresh_graph(const Node& new_root);
  
  const std::vector<std::shared_ptr<graph_node>> root_nodes() noexcept;

  bool has_selected_node() noexcept;

  std::weak_ptr<Node> selected_node();

  const std::vector<std::pair<std::shared_ptr<graph_node>, Edge>> 
  children(std::shared_ptr<graph_node> parent);

  const std::vector<std::pair<graph_node*, Edge>>
  parents(std::shared_ptr<graph_node> child);

  void add_child(std::shared_ptr<graph_node> parent&, 
                 const Node& child, const Edge& edge = Edge{});
  
  bool has(std::shared_ptr<graph_node> node);

  //
  // Below are four ways to breadth & depth search
  // If we are searching for a target, stopping & returning when it is found,
  // then we call the search a 'targeted search.'
  // If we are searching from a seed and not stopping we call it a 'seeded search.'
  // If the search is from a seed but stops on a target we refer to it as just a 'search.'
  // 
  // The four variations of search:
  // 1. (Seeded & Targeted) From a given node to a target node, return true
  //    if target node is found.
  // 2. (Targeted) From all root nodes to a target node, return true if target node is found.
  // 3. (Seeded) From a seed node performing on_touched & on_searched_hooks. 
  // 4. (Seeded) From all root nodes performing on_touched & on_searched_hooks.
  // All four methods allow on_touched and on_searched hooks to be given as lambdas.
  //
  // The four depth searches. (It doesn't look like lambdas as default args work so more
  // signartures were required.
  //
  template<class OnTouched, class OnSearched, class OnChild>
  bool depth_search(std::shared_ptr<graph_node> seed, std::shared_ptr<graph_node> target, 
           OnTouched on_touched = [](searchlist_subtype n){}, 
           OnSearched on_searched = [](searchlist_subtype n){},
           OnChild on_child = [](searchlist_subtype child, searchlist_subtype parent){});
  bool depth_search(std::shared_ptr<graph_node> seed, std::shared_ptr<graph_node> target);
  template<class OnTouched, class OnSearched, class OnChild>
  bool targeted_depth_search(std::shared_ptr<graph_node> target,
           OnTouched on_touched = [](searchlist_subtype n){},
           OnSearched on_searched = [](searchlist_subtype n){},
           OnChild on_child = [](searchlist_subtype child, searchlist_subtype parent){});
  bool targeted_depth_search(std::shared_ptr<graph_node> target);
  template<class OnTouched, class OnSearched, class OnChild>
  void seeded_depth_search(std::shared_ptr<graph_node> seed, OnTouched on_touched = [](searchlist_subtype n){}, 
           OnSearched on_searched = [](searchlist_subtype n){},
           OnChild on_child = [](searchlist_subtype child, searchlist_subtype parent){});
  void seeded_depth_search(std::shared_ptr<graph_node> seed);
  template<class OnTouched, class OnSearched, class OnChild>
  void seeded_depth_search(OnTouched on_touched = [](searchlist_subtype n){}, 
           OnSearched on_searched = [](searchlist_subtype n){},
           OnChild on_child = [](searchlist_subtype child, searchlist_subtype parent){});

  //
  // The four breadth searches.
  //
  template<class OnTouched, class OnSearched, class OnChild>
  bool breadth_search(std::shared_ptr<graph_node> seed, std::shared_ptr<graph_node> target, 
           OnTouched on_touched = [](searchlist_subtype n){},
           OnSearched on_searched = [](searchlist_subtype n){},
           OnChild on_child = [](searchlist_subtype child, searchlist_subtype parent){});
  bool breadth_search(std::shared_ptr<graph_node> seed, std::shared_ptr<graph_node> target);
  template<class OnTouched, class OnSearched, class OnChild>
  bool targeted_breadth_search(std::shared_ptr<graph_node> target,
           OnTouched on_touched = [](searchlist_subtype n){}, 
           OnSearched on_searched = [](searchlist_subtype n){},
           OnChild on_child = [](searchlist_subtype child, searchlist_subtype parent){});
  bool targeted_breadth_search(std::shared_ptr<graph_node> target);
  template<class OnTouched, class OnSearched, class OnChild>
  void seeded_breadth_search(std::shared_ptr<graph_node> seed, 
           OnTouched on_touched = [](searchlist_subtype n){}, 
           OnSearched on_searched = [](searchlist_subtype n){},
           OnChild on_child = [](searchlist_subtype child, searchlist_subtype parent){});
  void seeded_breadth_search(std::shared_ptr<graph_node> seed);
  template<class OnTouched, class OnSearched, class OnChild>
  void seeded_breadth_search(OnTouched on_touched = [](searchlist_subtype n){}, 
           OnSearched on_searched = [](searchlist_subtype n){},
           OnChild on_child = [](searchlist_subtype child, searchlist_subtype parent){});

  
  template<bool constant = false>
  class dfs_citerator
  {
   public:
    using pointer = typename std::conditional<constant, 
                                              const std::shared_ptr<graph_node>, 
                                              std::shared_ptr<graph_node>>::type;
    using reference = typename std::conditional<constant, const Node&, Node&>::type;
    //using value_type = typename std::conditional<constant, const Node, Node>::type;
    using difference_type = int;
    using iterator_category = std::input_iterator_tag;

    dfs_citerator() : t(nullptr) {}
    //dfs_citerator(const citerator& rhs) : t(rhs.t) {}
    dfs_citerator(pointer tt) : t(tt) {}
    //dfs_citerator(reference tt) : t(&tt) {}

    dfs_citerator operator++() { 
      
      ++t; return *this; 
    }
    dfs_citerator operator++(int) { dfs_citerator tmp(*this); operator++(); return tmp; }
    //dfs_citerator operator--() { --t; return *this; }
    //dfs_citerator operator--(int) { dfs_citerator tmp(*this); operator--(); return tmp; }
    
    reference operator*() { return *t; }
    bool operator==(const dfs_citerator& rhs) { return t == rhs.t; }
    bool operator!=(const dfs_citerator& rhs) { return t != rhs.t; }
    pointer ptr() const noexcept { return t; }

   protected:
    pointer t;
  };
  
  using iterator = dfs_citerator<false>;
  using const_iterator = dfs_citerator<true>;

  iterator begin() noexcept;

  const_iterator begin() const noexcept;

  iterator end() noexcept;

  const_iterator end() const noexcept;

 protected:
  std::vector<std::shared_ptr<graph_node>> the_root_nodes;

  std::weak_ptr<graph_node> the_selected_node;

  
  enum class search_status { unvisited = 0, touched, searched }; 
  using searchlist_subtype = std::pair<std::shared_ptr<graph_node>, Edge>;
   
  template<class C, bool targeted, class OnTouched, class OnSearched, class OnChild>
  bool search(std::shared_ptr<graph_node> seed, std::shared_ptr<graph_node> target,
              OnTouched on_touched = [](searchlist_subtype n){},
              OnSearched on_searched = [](searchlist_subtype n){},
              OnChild on_child = [](searchlist_subtype n){});
  template<class C, class OnTouched, class OnSearched, class OnChild>
  bool search(std::shared_ptr<graph_node> seed, std::shared_ptr<graph_node> target, 
              OnTouched on_touched = [](searchlist_subtype n){}, 
              OnSearched on_searched = [](searchlist_subtype n){},
              OnChild on_child = [](searchlist_subtype n){});  
  template<class C,class OnTouched, class OnSearched, class OnChild>
  bool targeted_search(std::shared_ptr<graph_node> target, 
              OnTouched on_touched = [](searchlist_subtype n){}, 
              OnSearched on_searched = [](searchlist_subtype n){},
              OnChild on_child = [](searchlist_subtype n){});
  template<class C, class OnTouched, class OnSearched, class OnChild>
  void seeded_search(std::shared_ptr<graph_node> seed, 
              OnTouched on_touched = [](searchlist_subtype n){}, 
              OnSearched on_searched = [](searchlist_subtype n){},
              OnChild on_child = [](searchlist_subtype n){});  
  template<class C, class OnTouched, class OnSearched, class OnChild>
  void seeded_search(OnTouched on_touched = [](searchlist_subtype n){}, 
              OnSearched on_searched = [](searchlist_subtype n){},
              OnChild on_child = [](searchlist_subtype n){});
  

};

template<class Node, class Edge>
fresh_graph<Node, Edge>::fresh_graph()
{
}
template<class Node, class Edge>
fresh_graph<Node, Edge>::fresh_graph(const Node& new_root)
 : fresh_graph()
{
  auto node = std::make_shared<graph_node>(new_root);
  // auto node = std::shared_ptr<graph_node>(graphnode, &graphnode->data); 
  the_root_nodes.push_back(node);
  the_selected_node = std::weak_ptr(node);
}
template<class Node, class Edge>
const std::vector<std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node>> 
fresh_graph<Node, Edge>::root_nodes() noexcept
{
  return the_root_nodes;
}
template<class Node, class Edge>
bool fresh_graph<Node, Edge>::has_selected_node() noexcept
{
  return !the_selected_node.is_expired();
}
template<class Node, class Edge>
std::weak_ptr<Node> fresh_graph<Node, Edge>::selected_node()
{
  return the_selected_node;
}

template<class Node, class Edge>
void fresh_graph<Node, Edge>::
add_child(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> parent, 
          const Node& child, const Edge& edge)
{
  parent->children.emplace_back(std::make_shared<graph_node>(child), edge);
}
template<class Node, class Edge>
const std::vector<std::pair<std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node>, Edge>>
fresh_graph<Node, Edge>::
children(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> parent)
{
  return parent->children;
}
template<class Node, class Edge>
const std::vector<std::pair<typename fresh_graph<Node, Edge>::graph_node*, Edge>> 
fresh_graph<Node, Edge>::
parents(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> child)
{
  return child->parents; 
}
template<class Node, class Edge>
bool fresh_graph<Node, Edge>::
has(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> node)
{
  return targeted_depth_search(node);
}

template<class Node, class Edge>
typename fresh_graph<Node, Edge>::iterator
fresh_graph<Node, Edge>::begin() noexcept
{
  if (!the_root_nodes.empty())
    return iterator(the_root_nodes.begin());
  else
    return iterator(nullptr);
}
template<class Node, class Edge>
typename fresh_graph<Node, Edge>::const_iterator
fresh_graph<Node, Edge>::begin() const noexcept
{
  if (!the_root_nodes.empty())
    return const_iterator(the_root_nodes.begin());
  else
    return const_iterator(nullptr);
}
template<class Node, class Edge>
typename fresh_graph<Node, Edge>::iterator
fresh_graph<Node, Edge>::end() noexcept
{
  return iterator(nullptr);
}
template<class Node, class Edge>
typename fresh_graph<Node, Edge>::const_iterator
fresh_graph<Node, Edge>::end() const noexcept
{
  return const_iterator(nullptr);
}

template<class Node, class Edge>
template<class OnTouched, class OnSearched, class OnChild>
bool fresh_graph<Node, Edge>::
depth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed, std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target,
             OnTouched on_touched, OnSearched on_searched, OnChild on_child)
{
  return search<std::stack<searchlist_subtype>>
           (seed, target, on_touched, on_searched, on_child);
}
template<class Node, class Edge>
bool fresh_graph<Node, Edge>::
depth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed, std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target)
{
  return depth_search(seed, target, [](auto n){}, [](auto n){}, [](auto c, auto p){});
}
template<class Node, class Edge>
template<class OnTouched, class OnSearched, class OnChild>
bool fresh_graph<Node, Edge>::
targeted_depth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target, OnTouched on_touched,
                      OnSearched on_searched, OnChild on_child)
{
  return targeted_search<std::stack<searchlist_subtype>>
           (target, on_touched, on_searched, on_child);
}
template<class Node, class Edge>
bool fresh_graph<Node, Edge>::
targeted_depth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target)
{
  return targeted_depth_search(target, [](auto n){}, [](auto n){}, [](auto c, auto p){});
}
template<class Node, class Edge>
template<class OnTouched, class OnSearched, class OnChild>
void fresh_graph<Node, Edge>::
seeded_depth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed, OnTouched on_touched,
                    OnSearched on_searched, OnChild on_child)
{
  seeded_search<std::stack<searchlist_subtype>>
           (seed, on_touched, on_searched, on_child);
} 
template<class Node, class Edge>
void fresh_graph<Node, Edge>::
seeded_depth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed)
{
  seeded_depth_search(seed, [](auto n){}, [](auto n){}, [](auto c, auto p){});
}
template<class Node, class Edge>
template<class OnTouched, class OnSearched, class OnChild>
void fresh_graph<Node, Edge>::
seeded_depth_search(OnTouched on_touched, OnSearched on_searched, OnChild on_child)
{
  seeded_search<std::stack<searchlist_subtype>>
           (on_touched, on_searched, on_child);
}

template<class Node, class Edge>
template<class OnTouched, class OnSearched, class OnChild>
bool fresh_graph<Node, Edge>::
breadth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed, std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target,
             OnTouched on_touched, OnSearched on_searched, OnChild on_child)
{
  return search<std::queue<searchlist_subtype>>
           (seed, target, on_touched, on_searched, on_child);
}
template<class Node, class Edge>
bool fresh_graph<Node, Edge>::
breadth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed, std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target)
{
  return breadth_search(seed, target, [](auto n){}, [](auto n){}, [](auto c, auto p){});
}
template<class Node, class Edge>
template<class OnTouched, class OnSearched, class OnChild>
bool fresh_graph<Node, Edge>::
targeted_breadth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target, OnTouched on_touched,
                        OnSearched on_searched, OnChild on_child)
{
  return targeted_search<std::queue<searchlist_subtype>>
           (target, on_touched, on_searched, on_child);
}
template<class Node, class Edge>
bool fresh_graph<Node, Edge>::
targeted_breadth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target)
{
  return targeted_breadth_search(target, [](auto n){}, [](auto n){}, [](auto c, auto p){});
}
template<class Node, class Edge>
template<class OnTouched, class OnSearched, class OnChild>
void fresh_graph<Node, Edge>::
seeded_breadth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed, OnTouched on_touched,
                      OnSearched on_searched, OnChild on_child)
{
  seeded_search<std::queue<searchlist_subtype>>
           (seed, on_touched, on_searched, on_child);
}
template<class Node, class Edge>
void fresh_graph<Node, Edge>::
seeded_breadth_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed)
{
  seeded_breadth_search(seed, [](auto n){}, [](auto n){}, [](auto c, auto p){});
}
template<class Node, class Edge>
template<class OnTouched, class OnSearched, class OnChild>
void fresh_graph<Node, Edge>::
seeded_breadth_search(OnTouched on_touched, OnSearched on_searched, OnChild on_child)
{
  seeded_search<std::queue<searchlist_subtype>>
           (on_touched, on_searched, on_child);
}

template<class Node, class Edge>
template<class C, bool targeted, class OnTouched, class OnSearched, class OnChild>
bool fresh_graph<Node, Edge>::
search(const std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed, const std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target, 
       OnTouched on_touched, OnSearched on_searched, OnChild on_child)
{
  C searchlist;
  std::unordered_map<std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node>, search_status> node_status_map;
  searchlist.push({seed, Edge{}});
  while (!searchlist.empty()) {
    searchlist_subtype current_item = head(searchlist);
    std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> current_node = current_item.first;
    if constexpr(targeted)
      if (current_node == target) return true;
    node_status_map[current_node] = search_status::touched;
    auto next_children = children(current_node);
    on_touched(pop(searchlist));
    for (auto& child : next_children) {
      on_child(child, current_item);
      if (node_status_map[child.first] == search_status::unvisited) {
        searchlist.push(child);
      }
    }
    node_status_map[current_node] = search_status::searched;
    on_searched(current_item);
  }
  return false;
}
template<class Node, class Edge>
template<class C, class OnTouched, class OnSearched, class OnChild>
bool fresh_graph<Node, Edge>::
search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed, std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target, 
       OnTouched on_touched, OnSearched on_searched, OnChild on_child)
{
  return search<C, true>(seed, target, on_touched, on_searched, on_child);
}
template<class Node, class Edge>
template<class C, class OnTouched, class OnSearched, class OnChild>
bool fresh_graph<Node, Edge>::
targeted_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> target, 
                OnTouched on_touched, OnSearched on_searched, OnChild on_child)
{
  for (auto& root_node : root_nodes())
    if (search<C>(root_node, target, on_touched, on_searched, on_child)) return true;
  return false;
}
template<class Node, class Edge>
template<class C, class OnTouched, class OnSearched, class OnChild>
void fresh_graph<Node, Edge>::
seeded_search(std::shared_ptr<typename fresh_graph<Node, Edge>::graph_node> seed,
              OnTouched on_touched, OnSearched on_searched, OnChild on_child)
{
  search<C, false>(seed, nullptr, on_touched, on_searched, on_child);
}
template<class Node, class Edge>
template<class C, class OnTouched, class OnSearched, class OnChild>
void fresh_graph<Node, Edge>::
seeded_search(OnTouched on_touched, OnSearched on_searched, OnChild on_child)
{
  for (auto& root_node : root_nodes())
    seeded_search<C>(root_node, on_touched, on_searched, on_child);
}


//template<class Node, class Edge>
//fresh_graph<Node, Edge>::
//template<class Node, class Edge>
//fresh_graph<Node, Edge>::
//template<class Node, class Edge>
//fresh_graph<Node, Edge>::
//template<class Node, class Edge>
//fresh_graph<Node, Edge>::
//template<class Node, class Edge>
//fresh_graph<Node, Edge>::

template<class N, class E>
std::ostream& operator<<(std::ostream& os, fresh_graph<N, E>& g)
{
  //
  // TODO: fix this implementation, DFS won't show all relationships
  // we to show all children of each node starting from the parent 
  //
  g.seeded_depth_search([](auto p){ },
                        [](auto p){ },
                        [&os](auto child, auto parent){ 
                          os << parent.first->data << "--" << child.second;
                          os << "-->" << child.first->data << "\n";
                        });
  return os;
}

} // namespace ryk

#endif 
