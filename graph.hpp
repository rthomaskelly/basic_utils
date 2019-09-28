#ifndef ryk_graph
#define ryk_graph

#include <unordered_map>

#include "iterable_utils.hpp"
#include "algorithm_extras.hpp"

#include <iostream>
#include <boost/lexical_cast.hpp>
using std::cout;
using std::endl;

namespace ryk {

// general note:
// it would be nice to have a term denoting a node/edge pair (std::pair<Node, Edge>)
// I have used 'ray' below, not sure if a better one exists
// another note:
// a graph has some interesting semantics around removal
// because we have the question of what to do with its children
// we could remove a node and then get rid of all its children (and their children)
// or we could remove a node and attach its children to its former parent
// which is more useful, or rather more common, I am unsure of
// but I think the second option of keeping t 
template<class Node, class Edge, class Hash = std::hash<Node>>
class directed_graph
{
 public:
  directed_graph();

  directed_graph(const Node& new_root);

  std::vector<Node> root_nodes() const noexcept;
  
  bool has_selected_node() const noexcept;

  const Node& selected_node() const;
  
  void add_child(const Node& parent, const Node& child, const Edge& edge = Edge{});
  
  void add_child_to_selected(const Node& child, const Edge& edge = Edge{});
  
  void attach(const Node& parent, const directed_graph& g, const Edge& edge = Edge{});

  void append(const directed_graph& g, const Edge& edge = Edge{});
  
  const std::vector<std::pair<Node, Edge>> children(const Node& parent) const;
  
  const std::vector<std::pair<Node, Edge>> parents(const Node& child) const;

  std::size_t size() const noexcept;
  
  bool empty() const noexcept;
  
  auto find_child(const Node& parent, const Node& child) const;
  
  bool has_child(const Node& parent, const Node& child) const;
  
  Edge edge_between(const Node& parent, const Node& child);
  
  void replace(const Node& node, const Node& parent, 
               const directed_graph& replacement, const Edge& new_edge);
  
  void replace(const Node& node, const Node& parent, const directed_graph& replacement);

  //
  // pluck trim and remove all do removals
  // pluck connects the nodes children to its parents and removes the node
  // the children are connected by the edges they had to the plucked node
  //
  void pluck(const Node& node); 

  //
  // trim removes the node and deletes all of its children
  //
  void trim(const Node& node);
  
  //
  // remove deletes the node potentially causing the graph to splice
  // i.e. if the removed node was the sole parent of any of its children
  // those children will be orphaned and thus become root nodes
  //
  void remove(const Node& node);
  
  bool depth_search(const Node& target);
  
  bool breadth_search(const Node& target);
  
  directed_graph find_path(const Node& startnode, const Node& endnode);

  template<class N, class E, class H>
  friend std::ostream& operator<<(std::ostream&, const directed_graph<N, E, H>& g);
  
  bool operator==(const directed_graph& rhs) const noexcept;

  template<bool constant = false>
  class citerator
  {
   public:
    using pointer = typename std::conditional<constant, const Node*, Node*>::type;
    using reference = typename std::conditional<constant, const Node&, Node&>::type;
    using value_type = typename std::conditional<constant, const Node, Node>::type;
    using difference_type = int;
    using iterator_category = std::input_iterator_tag;

    citerator() : t(nullptr) {}
    citerator(const citerator& rhs) : t(rhs.t) {}
    citerator(pointer tt) : t(tt) {}
    citerator(reference tt) : t(&tt) {}

    citerator operator++() { ++t; return *this; }
    citerator operator++(int) { citerator tmp(*this); operator++(); return tmp; }
    citerator operator--() { --t; return *this; }
    citerator operator--(int) { citerator tmp(*this); operator--(); return tmp; }
    
    reference operator*() { return *t; }
    bool operator==(const citerator& rhs) { return t == rhs.t; }
    bool operator!=(const citerator& rhs) { return t != rhs.t; }
    pointer ptr() const noexcept { return t; }

   protected:
    pointer t;
  };
  
  using iterator = citerator<false>;
  using const_iterator = citerator<true>;

  iterator begin() noexcept;

  const_iterator begin() const noexcept;

  iterator end() noexcept;

  const_iterator end() const noexcept;
 
protected:
  std::unordered_map<Node, std::vector<std::pair<Node, Edge>>, Hash> child_map;
  std::unordered_map<Node, std::vector<std::pair<Node, Edge>>, Hash> parent_map;
  Node the_selected_node;
  bool has_a_selected_node;
  
  enum class search_status { unvisited = 0, touched, searched }; 

  template<class C>
  bool targeted_search(const Node& target, const Node& starting_node);

  // this ought to be implemented as iterator increment
  // i.e. moving through iterators will do this full search allowing the user to put
  // code in for 'f' as opposed to just a lambda
  template<class C, class Fn>
  void full_search(const Node& starting_node, Fn f);
};

template<class Node, class Edge, class Hash>
directed_graph<Node, Edge, Hash>::directed_graph()
 : has_a_selected_node(false)
{
}
template<class Node, class Edge, class Hash>
directed_graph<Node, Edge, Hash>::directed_graph(const Node& new_root)
 : the_selected_node(new_root), has_a_selected_node(true)
{
  child_map[new_root] = std::vector<std::pair<Node, Edge>>{};
  parent_map[new_root] = std::vector<std::pair<Node, Edge>>{};
}
template<class Node, class Edge, class Hash>
std::vector<Node> directed_graph<Node, Edge, Hash>::root_nodes() const noexcept
{ 
  std::vector<Node> the_roots;
  for (auto& parent_children_pair : parent_map)
   if (parent_children_pair.second.empty()) 
     the_roots.push_back(parent_children_pair.first);   
  
  return the_roots;
}
template<class Node, class Edge, class Hash>
bool directed_graph<Node, Edge, Hash>::has_selected_node() const noexcept
{ 
  return has_a_selected_node; 
}
template<class Node, class Edge, class Hash>
const Node& directed_graph<Node, Edge, Hash>::selected_node() const
{ 
  if (!has_a_selected_node) {
    throw std::runtime_error("Tried to call ryk::graph::selected_node() for graph"
          " that has no node selected. \nGraph: " + boost::lexical_cast<std::string>(*this));
  }
  return the_selected_node;
}
template<class Node, class Edge, class Hash>
void directed_graph<Node, Edge, Hash>::add_child(const Node& parent,
                                                 const Node& child, const Edge& edge)
{
  // TODO: add check for cycles
  child_map[parent].push_back(std::make_pair(child, edge));
  child_map.insert({child, std::vector<std::pair<Node, Edge>>{}});
  parent_map[child].push_back(std::make_pair(parent, edge));
}
template<class Node, class Edge, class Hash>
void directed_graph<Node, Edge, Hash>::add_child_to_selected(const Node& child, const Edge& edge)
{
  if (!has_a_selected_node) {
    throw std::runtime_error("Tried to ryk::graph::add_child_to_selected() without any node"
          " in the graph being selected. Adding child node '"
          + boost::lexical_cast<std::string>(child) + "' across edge '" 
          + boost::lexical_cast<std::string>(edge) + "'.\n To graph "
          + boost::lexical_cast<std::string>(*this));
  }
  add_child(the_selected_node, child, edge);
}
template<class Node, class Edge, class Hash>
void directed_graph<Node, Edge, Hash>::attach(const Node& parent, const directed_graph& g, 
                                              const Edge& edge)
{
  Node current_parent = parent;
  std::queue<std::pair<Node, Edge>> search_list;
  for (auto& root_node : g.root_nodes()) {
    search_list.push(std::make_pair(root_node, edge));
    while(!search_list.empty()) {
      add_child(current_parent, head(search_list).first, head(search_list).second);
      for (const auto& child : g.children(pop(search_list).first)) search_list.push(child);
    }
  }
  if (g.has_a_selected_node) {
    the_selected_node = g.the_selected_node;
    has_a_selected_node = true;
  }
}
template<class Node, class Edge, class Hash>
void directed_graph<Node, Edge, Hash>::append(const directed_graph& g, const Edge& edge)
{
  if (has_a_selected_node) attach(the_selected_node, g, edge);
  else if (child_map.empty() && parent_map.empty()) *this = g;
  else // a case for when there is no selected node of a non-empty graph needs to be handled
  {
    throw std::runtime_error("Tried to append() to a non-empty graph with no selected node.");
  }
}
template<class Node, class Edge, class Hash>
const std::vector<std::pair<Node, Edge>> 
directed_graph<Node, Edge, Hash>::children(const Node& parent) const 
{ 
  if (has(child_map, parent)) return child_map.at(parent);
  else return std::vector<std::pair<Node, Edge>>{};
}
template<class Node, class Edge, class Hash>
const std::vector<std::pair<Node, Edge>> 
directed_graph<Node, Edge, Hash>::parents(const Node& child) const 
{
  if (has(parent_map, child)) return parent_map.at(child);
  else return std::vector<std::pair<Node, Edge>>{};
}
template<class Node, class Edge, class Hash>
std::size_t directed_graph<Node, Edge, Hash>::size() const noexcept
{
  return parent_map.size();
}
template<class Node, class Edge, class Hash>
bool directed_graph<Node, Edge, Hash>::empty() const noexcept
{
  return parent_map.empty();
}
template<class Node, class Edge, class Hash>
auto directed_graph<Node, Edge, Hash>::find_child(const Node& parent, const Node& child) const 
{
  if (has(child_map, parent)) {
    auto siblings = children(parent);
    auto iter = find_if(siblings, [&child](const auto& e){ return e == child; });
    if (iter != siblings.end()) return iter;
    else throw std::out_of_range("tried to find_child() for non-existent parent-child combination.");
  } else throw std::out_of_range("tried to find_child() for non-existent parent node.");
}
template<class Node, class Edge, class Hash>
bool directed_graph<Node, Edge, Hash>::has_child(const Node& parent, const Node& child) const
{
  if (has(child_map, parent)) {
    if (has_if(children(parent), [&child](const auto& e){ return e == child; })) {
      return true;
    }
  } 
  return false;
}
template<class Node, class Edge, class Hash>
Edge directed_graph<Node, Edge, Hash>::edge_between(const Node& parent, const Node& child)
{
  // this will need to be an edge iterator to work w/o errors (we can't return not found)
  auto iter = find_child(parent, child);
  return iter->second;
}
template<class Attach>
template<class Node, class Edge, class Hash>
void directed_graph<Node, Edge, Hash>::replace(const Node& node, 
                                               const directed_graph& replacement, 
                                               Attach attach)
{
  // vocab:
  // target graph -> the *this graph
  // target node -> here 'node'
  // replacement -> here replacement
  // specific: the definition -> the replacement
  // attaches -> a type of node in the replacement, evaluated by lambda is_attaches
  // specific: placeholder -> an attaches in the definition
  // target descendents ->


  // for my def case:
  // need to replace the placeholders in the definition (here replacement) 
  // these placeholders correspond to the main graph by edgename from the replacing node
  // attach is (children, graph)->void
  auto the_children = children(node);
   
  
} 
template<class Node, class Edge, class Hash>
void directed_graph<Node, Edge, Hash>::replace(const Node& node, const Node& parent, 
                                               const directed_graph& replacement, 
                                               const Edge& new_edge)
{
  remove(node);
  attach(parent, replacement, new_edge);
}
template<class Node, class Edge, class Hash>
void directed_graph<Node, Edge, Hash>::replace(const Node& node, const Node& parent,
                                               const directed_graph& replacement)
{
  replace(node, parent, replacement, edge_between(parent, node));    
}
template<class Node, class Edge, class Hash>
void pluck(const Node& node)
{
  auto the_parents = parents(node);
  auto the_children = children(node);
  for (auto& parent : the_parents) {
    for (auto& child : the_children) {
      // connect the plucked node's children to its parents
      child_map[parent.first].push_back(child);
      parent_map[child.first].push_back(parent);
    } 
  }
  remove(node);
}
template<class Node, class Edge, class Hash>
void trim(const Node& node)
{
  // dfs removing child after searched
  full_search(node, [](Node n){}, [](const Node& n){ remove(n); });
}
template<class Node, class Edge, class Hash>
void directed_graph<Node, Edge, Hash>::remove(const Node& node)
{
  if (has(child_map, node)) child_map.erase(child_map.find(node))

  // remove it as a child from parents
  if (has(parent_map, node)) {
    for (auto& parent : parent_map[node]) {
      auto& siblings = child_map.at(parent.first);
      erase_remove_if(siblings, [&node](auto pair){ return pair.first == node; });
    }
    parent_map.erase(parent_map.find(node));
  }
}
template<class Node, class Edge, class Hash>
bool directed_graph<Node, Edge, Hash>::depth_search(const Node& target)
{
  for (const auto& root_node : root_nodes())
    if (targeted_search<std::stack<Node>>(target, root_node)) return true;
  return false; 
}
template<class Node, class Edge, class Hash>  
bool directed_graph<Node, Edge, Hash>::breadth_search(const Node& target)
{
  for (const auto& root_node : root_nodes())
    if (targeted_search<std::queue<Node>>(target, root_node)) return true;
  return false;
}
template<class Node, class Edge, class Hash> 
directed_graph<Node, Edge, Hash> 
directed_graph<Node, Edge, Hash>::find_path(const Node& startnode, 
                                            const Node& endnode)
{
  using ray = std::pair<Node, Edge>;
  auto ray_gt = [](const ray& lp, const ray& rp){ return lp.second > rp.second; };
  auto the_queue = std::priority_queue<ray, std::vector<ray>, decltype(ray_gt)>{};
  auto the_path = directed_graph<Node, Edge, Hash>{};
  if (startnode == endnode) return the_path;
  the_queue.push(startnode);
  while(!the_queue.empty()) {
    for (const auto& children : children(the_queue.top())) {
       
    }
  }  
}
template<class Node, class Edge, class Hash>
typename directed_graph<Node, Edge, Hash>::iterator
directed_graph<Node, Edge, Hash>::begin() noexcept
{

}
template<class Node, class Edge, class Hash>
typename directed_graph<Node, Edge, Hash>::const_iterator
directed_graph<Node, Edge, Hash>::begin() const noexcept
{

}
template<class Node, class Edge, class Hash>
typename directed_graph<Node, Edge, Hash>::iterator
directed_graph<Node, Edge, Hash>::end() noexcept
{

}
template<class Node, class Edge, class Hash>
typename directed_graph<Node, Edge, Hash>::const_iterator
directed_graph<Node, Edge, Hash>::end() const noexcept
{

}

template<class Node, class Edge, class Hash>
template<class C>
bool directed_graph<Node, Edge, Hash>::targeted_search(const Node& target, 
                                                       const Node& starting_node)
{
  C search_list;
  std::unordered_map<Node, search_status, Hash> node_status_map;
  search_list.push(starting_node);
  while (!search_list.empty()) {
    Node current_node = head(search_list);
    if (head(search_list) == target) return true;
    node_status_map[current_node] = search_status::touched;
    cout << head(search_list) << " ";
    for (const auto& child : children(pop(search_list))) {
      if (node_status_map[child.first] == search_status::unvisited)
        search_list.push(child.first);
    }
    node_status_map[current_node] = search_status::searched;
  }
  return false;
}

  // this ought to be implemented as iterator increment
  // i.e. moving through iterators will do this full search allowing the user to put
  // code in for 'f' as opposed to just a lambda
template<class Node, class Edge, class Hash>
template<class C, class Fn0, class Fn1>
void directed_graph<Node, Edge, Hash>::full_search(const Node& starting_node, Fn0 on_touched, 
                                                   Fn1 on_searched = [](Node n){})
{
  C search_list;
  std::unordered_map<Node, search_status, Hash> node_status_map;
  search_list.push(starting_node);
  while (!search_list.empty()) {
    Node current_node = head(search_list);
    auto next_children = children(head(search_list));
    node_status_map[current_node] = search_status::touched;
    on_touch(pop(search_list));
    for (const auto& child : next_children) { 
      if (node_status_map[child.first] == search_status::unvisited)
        search_list.push(child.first);
    }
    node_status_map[current_node] = search_status::searched;
    on_searched(current_node);
  }
}

template<class N, class E, class H>
std::ostream& operator<<(std::ostream& os, const directed_graph<N, E, H>& g)
{
  for (const auto& root : g.root_nodes()) {
    os << "root " << root;
    std::stack<std::pair<N, E>> search_list;
    std::unordered_map<N, typename directed_graph<N,E,H>::search_status, H> node_status_map;
    search_list.push(std::make_pair(root, E{}));
    do {
      N current_parent = head(search_list).first;
      node_status_map[current_parent] = directed_graph<N, E, H>::search_status::touched;
      auto next_children = g.children(pop(search_list).first);
      for (const auto& child : next_children) {
        if (node_status_map[child.first] == directed_graph<N, E, H>::search_status::unvisited) {
          search_list.push(child);
          os << "\n" << current_parent << " --" << child.second << "--> " << child.first;
        }
      }
      node_status_map[current_parent] = directed_graph<N, E, H>::search_status::searched;
    } while (!search_list.empty());
  }
  return os;
}

template<class Node, class Edge, class Hash>
bool directed_graph<Node, Edge, Hash>::
operator==(const directed_graph<Node, Edge, Hash>& rhs) const noexcept
{
  return (child_map == rhs.child_map) && (parent_map == rhs.parent_map);
}

} // namespace ryk

#endif
