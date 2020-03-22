
#include <vector>
#include <map>
#include <set>
#include <list>
#include <array>

#include "gtest/gtest.h"

#include "iterable_algorithms.hpp"
#include "statistics.hpp"
#include "graph_deferred.hpp"

#include "dynamic/Exp.hpp"

using namespace std::string_literals;

TEST(Traits, is_iterable)
{
  static_assert(!ryk::is_iterable_v<int>);
  static_assert(!ryk::is_iterable_v<double>);
  static_assert(!ryk::is_iterable_v<std::pair<int, double>>);
  static_assert(!ryk::is_iterable_v<std::tuple<int, double, char*>>);
  static_assert(!ryk::is_iterable_v<char*>);

  static_assert(ryk::is_iterable_v<std::string>);
  static_assert(ryk::is_iterable_v<std::vector<int>>);
  static_assert(ryk::is_iterable_v<std::list<double>>);
  static_assert(ryk::is_iterable_v<std::map<int, double>>);
  static_assert(ryk::is_iterable_v<std::unordered_map<int, double>>);
}

TEST(Traits, is_pointerlike)
{
  const char* c_str = "testing";
  // auto* a = ++c_str;
  // a = c_str++;
  auto c = *(c_str + 2);
  
  EXPECT_EQ(c, 's');

  // why does this fail?
  // static_assert(ryk::is_pointerlike_v<decltype(c_str)>);
}

TEST(Traits, is_iterator)
{
  std::vector<int> v;
  static_assert(ryk::is_iterator_v<decltype(v.begin())>);
  static_assert(ryk::is_iterator_v<decltype(v.end())>);
  static_assert(!ryk::is_iterator_v<decltype(v)>);

  std::string s;
  static_assert(ryk::is_iterator_v<decltype(s.begin())>);
  static_assert(ryk::is_iterator_v<decltype(s.end())>);
  static_assert(!ryk::is_iterator_v<decltype(s)>);
  
}

TEST(IterableAlgorithms, find)
{
  std::map<int, int> m{ {1, 1}, {2, 2}, };
  std::vector<int> v{0, 1, 2};
  std::list<int> l{0, 1, 2};
  std::array<int, 3> a{0, 1, 2}; 

  ASSERT_NE(ryk::find(m, 1), m.end());
  ASSERT_NE(ryk::find(v, 1), v.end());
  ASSERT_NE(ryk::find(l, 1), l.end());
  ASSERT_NE(ryk::find(a, 1), a.end());

  // Having a tough time getting the below to compile.
  // auto p = std::make_pair<const int, int>(1, 1);
  // EXPECT_EQ(*ryk::find(m, 1), p);
  EXPECT_EQ(*ryk::find(v, 1), 1);
  EXPECT_EQ(*ryk::find(l, 1), 1);
  EXPECT_EQ(*ryk::find(a, 1), 1);

  EXPECT_EQ(ryk::find(m, 5), m.end());
  EXPECT_EQ(ryk::find(v, 5), v.end());
  EXPECT_EQ(ryk::find(l, 5), l.end());
  EXPECT_EQ(ryk::find(a, 5), a.end());
}

TEST(IterableAlgorithms, find_index)
{
  std::map<int, int> m{ {1, 1}, {2, 2}, };
  std::vector<int> v{0, 1, 2};
  std::list<int> l{0, 1, 2};
  std::array<int, 3> a{0, 1, 2}; 

  EXPECT_EQ(ryk::find_index(m, 1), 0);
  EXPECT_EQ(ryk::find_index(v, 1), 1);
  EXPECT_EQ(ryk::find_index(l, 1), 1);
  EXPECT_EQ(ryk::find_index(a, 1), 1);
}

TEST(IterableAlgorithms, search)
{
  std::map<int, int> m{ {1, 1}, {2, 2}, {3, 3}, {4, 4}, };
  std::map<int, int> m34{ {3, 3}, {4, 4}, };
  std::map<int, int> m3{ {3, 3}, };
  std::map<int, int> m24{ {2, 2}, {4, 4}, };
  std::vector<int> v{0, 1, 2, 3, 4};
  std::vector<int> v34{3, 4};
  std::vector<int> v13{1, 3};
  std::list<int> l{0, 1, 2, 3, 4};
  std::list<int> l34{3, 4};

  EXPECT_NE(ryk::search(m, m34), m.end());
  EXPECT_NE(ryk::search(m, m3), m.end());
  EXPECT_EQ(ryk::search(m, m24), m.end());

  EXPECT_NE(ryk::search(v, v34), v.end());
  EXPECT_EQ(ryk::search(v, v13), v.end());
  EXPECT_NE(ryk::search(v, l34), v.end());

  EXPECT_NE(ryk::search(l, v34), l.end());
  EXPECT_EQ(ryk::search(l, v13), l.end());
  EXPECT_NE(ryk::search(l, l34), l.end());

  EXPECT_NE(ryk::search(m, l34, [](auto pair, auto i){ return pair.first == i; }), m.end());
  EXPECT_NE(ryk::search(m, v34, [](auto pair, auto i){ return pair.first == i; }), m.end());
  EXPECT_EQ(ryk::search(m, v13, [](auto pair, auto i){ return pair.first == i; }), m.end());
}

TEST(IterableAlgorithms, sort)
{
  std::vector<int> v{5, 4, 3, 2, 1, 0};
  std::list<int> l{5, 4, 3, 2, 1, 0};
  std::set<int> s{5, 4, 3, 2, 1, 0};

  std::vector<int> vs{0, 1, 2, 3, 4, 5};
  std::list<int> ls{0, 1, 2, 3, 4, 5};
  std::set<int> ss{0, 1, 2, 3, 4, 5};
  
  EXPECT_EQ(ryk::sort(v), vs);
  EXPECT_EQ(ryk::sort(l), ls);
  EXPECT_EQ(ryk::sort(s), ss);
}

TEST(IterableAlgorithms, accumulate_vector)
{
  const std::vector<int> c{1, 2, 3, 4};
  std::vector<int> cE{};
  std::vector<int> c2{2}; 
  std::vector<std::string> cs{"this", "is", "a", "test"};
  
  EXPECT_EQ(ryk::accumulate(c), 10);
  EXPECT_EQ(ryk::accumulate(cE), int{});
  EXPECT_EQ(ryk::accumulate(c2), 2);
  EXPECT_EQ(ryk::accumulate(cs), "thisisatest"s);
  EXPECT_EQ(ryk::accumulate(c, [](auto a, auto b){ return a * b; }), 24);
  EXPECT_EQ(ryk::accumulate(cE, [](auto a, auto b){ return a * b; }), int{});
  EXPECT_EQ(ryk::accumulate(c2, [](auto a, auto b){ return a * b; }), 2);
}

TEST(IterableAlgorithms, accumulate_list)
{
  std::list<int> c{1, 2, 3, 4};
  std::list<int> cE{};
  std::list<int> c2{2}; 
  std::list<std::string> cs{"this", "is", "a", "test"};
  
  EXPECT_EQ(ryk::accumulate(c), 10);
  EXPECT_EQ(ryk::accumulate(cE), int{});
  EXPECT_EQ(ryk::accumulate(c2), 2);
  EXPECT_EQ(ryk::accumulate(cs), "thisisatest"s);
  EXPECT_EQ(ryk::accumulate(c, [](auto a, auto b){ return a * b; }), 24);
  EXPECT_EQ(ryk::accumulate(cE, [](auto a, auto b){ return a * b; }), int{});
  EXPECT_EQ(ryk::accumulate(c2, [](auto a, auto b){ return a * b; }), 2);
}

TEST(IterableAlgorithms, accumulate_set)
{
  std::set<int> c{1, 2, 3, 4};
  std::set<int> cE{};
  std::set<int> c2{2}; 
  std::set<std::string> cs{"this", "is", "a", "test"};
  
  EXPECT_EQ(ryk::accumulate(c), 10);
  EXPECT_EQ(ryk::accumulate(cE), int{});
  EXPECT_EQ(ryk::accumulate(c2), 2);
  EXPECT_EQ(ryk::accumulate(cs), "aistestthis"s);
  EXPECT_EQ(ryk::accumulate(c, [](auto a, auto b){ return a * b; }), 24);
  EXPECT_EQ(ryk::accumulate(cE, [](auto a, auto b){ return a * b; }), int{});
  EXPECT_EQ(ryk::accumulate(c2, [](auto a, auto b){ return a * b; }), 2);
}

TEST(IterableAlgorithms, slice_vector)
{
  std::vector<int> c{0, 1, 2, 3, 4, 5};
  std::vector<int> slice1{2, 3, 4};
  std::vector<int> slice2{5};
  std::vector<int> slice3{4, 1, 2};
  std::vector<std::string> cs{"this", "is", "a", "test"};
  std::vector<int> slice4{2, 1};
  std::vector<std::string> cs_slice4_eq{"a", "is"};
   
  
  EXPECT_EQ(ryk::slice(c, slice1), slice1);
  EXPECT_EQ(ryk::slice(c, slice2), slice2);
  EXPECT_EQ(ryk::slice(c, slice3), slice3);
  EXPECT_EQ(ryk::slice(cs, slice4), cs_slice4_eq); 
  // EXPECT_EQ(ryk::slice(cs, {2, 1}), cs_slice4_eq); this init-list version doesn't compile
}

TEST(IterableAlgorithms, slice_list)
{
  std::list<int> c{0, 1, 2, 3, 4, 5};
  std::vector<int> slice1{2, 3, 4};
  std::vector<int> slice2{5};
  std::list<int> slice3{4, 1, 2};
  std::list<std::string> cs{"this", "is", "a", "test"};
  std::vector<int> slice4{2, 1};

  std::list<std::string> cs_slice4_eq{"a", "is"};
  std::list<int> c_slice1_eq{2, 3, 4};
  std::list<int> c_slice2_eq{5};
   
  
  EXPECT_EQ(ryk::slice(c, slice1), c_slice1_eq);
  EXPECT_EQ(ryk::slice(c, slice2), c_slice2_eq);
  EXPECT_EQ(ryk::slice(c, slice3), slice3);
  EXPECT_EQ(ryk::slice(cs, slice4), cs_slice4_eq); 
}

TEST(IterableAlgorithms, slice_set)
{
  std::set<int> c{0, 1, 2, 3, 4, 5};
  std::set<int> slice1{2, 3, 4};
  std::vector<int> slice2{5};
  std::list<int> slice3{4, 1, 2};
  std::set<std::string> cs{"this", "is", "a", "test"};
  std::vector<int> slice4{2, 1};

  std::set<std::string> cs_slice4_eq{"is", "test"};
  std::set<int> c_slice2_eq{5};
  std::set<int> c_slice3_eq{1, 2, 4};
  
  EXPECT_EQ(ryk::slice(c, slice1), slice1);
  EXPECT_EQ(ryk::slice(c, slice2), c_slice2_eq);
  EXPECT_EQ(ryk::slice(c, slice3), c_slice3_eq);
  EXPECT_EQ(ryk::slice(cs, slice4), cs_slice4_eq); 
}

TEST(Statistics, mean)
{
  std::vector<int> v{0, 1, 2, 3, 4};
  std::list<int> l{1, 2, 3, 4, 5};
  std::set<int> s{0, 1, 2, 3, 4};

  EXPECT_EQ(ryk::mean(v), 2);
  EXPECT_EQ(ryk::mean(l), 3);
  EXPECT_EQ(ryk::mean(s), 2);
}

TEST(Statistics, median)
{
  std::vector<int> v{0, 2, 3, 1, 4};
  std::list<int> l{1, 2, 4, 3, 5};
  std::set<int> s{4, 1, 2, 3, 0};
  std::vector<double> vd{1, 2, 3, 4};
  std::vector<int> vE{};
  std::vector<int> v1{1};
  
  EXPECT_EQ(ryk::median(v), 2);
  EXPECT_EQ(ryk::median(l), 3);
  EXPECT_EQ(ryk::median(s), 2);  
  EXPECT_EQ(ryk::median(vd), 2.5);  
  EXPECT_EQ(ryk::median(vE), 0);  
  EXPECT_EQ(ryk::median(v1), 1);  
}

TEST(Statistics, quartiles)
{
  std::vector<int> v(101);
  ryk::iota(v);
  
  auto the_quartiles = ryk::quartiles(v);
  EXPECT_EQ(the_quartiles[0], 0);
  EXPECT_EQ(the_quartiles[25], 25);
  EXPECT_EQ(the_quartiles[50], 50);
  EXPECT_EQ(the_quartiles[75], 75);
  EXPECT_EQ(the_quartiles[100], 100);
}

TEST(DeferredGraph, graph)
{
  ryk::deferred_graph<int, int> g(0);
  auto& roots = g.root_nodes();
  gcpp::deferred_ptr<ryk::deferred_graph<int, int>::graph_node> root = roots.at(0); 

  auto& pair1 = g.add_child(root, 1);
  auto& pair2 = g.add_child(root, 2);
  g.add_child(root, 3);

  auto& pair10 = g.add_child(pair1.first, 10);
  auto& pair11 = g.add_child(pair1.first, 11);
  auto& pair110 = g.add_child(pair11.first, 110);
  g.add_child(pair110.first, 1100);

  g.add_child(pair10.first, 1100);
  auto& pair20 = g.add_child(pair2.first, 20);
  auto& pair21 = g.add_child(pair2.first, 21);
  g.add_child(pair20.first, 200);
  g.add_child(pair21.first, 210);
  g.add_child(pair21.first, 211);
  
  // ex syntax:
  // auto ptr = ryk::the_graph_heap.make<ryk::deferred_graph<int, int>::graph_node>(10);
 
  auto node_dptr = g.targeted_depth_search(20);
  EXPECT_NE(node_dptr, nullptr);
  EXPECT_EQ(node_dptr->data(), 20);

  node_dptr = g.find(20);
  EXPECT_NE(node_dptr, nullptr);
  EXPECT_EQ(node_dptr->data(), 20);
  
  //
  // test attach()
  std::vector<int> rootv{30, 31};
  ryk::deferred_graph<int, int> sub_g(rootv);
  auto dptr_subg30 = sub_g.find(30);  
  EXPECT_NE(dptr_subg30, nullptr);
  EXPECT_EQ(dptr_subg30->data(), 30);
 
  sub_g.add_child(dptr_subg30, 300);

  auto dptr_g3 = g.find(3);
  EXPECT_NE(dptr_g3, nullptr);
  EXPECT_EQ(dptr_g3->data(), 3); 
  g.attach(dptr_g3, sub_g);
  
  auto dptr_g300 = g.find(300);
  EXPECT_NE(dptr_g300, nullptr);
  EXPECT_EQ(dptr_g300->data(), 300);

  //
  // test append()
  // CURRENTLY DOES NOT COMPILE b/c ASSIGNMENT OPERATOR '=' USED IN 'append' 
  //ryk::deferred_graph<int, int> sub_g2(3000);
  //g.append(sub_g2);
  //auto dptr_g3000 = g.find(3000);
  //EXPECT_NE(dptr_g3000, nullptr);
  //EXPECT_EQ(dptr_g3000->data(), 3000);
}

TEST(dlist, dlist)
{
  ryk::dlist l;
  l.push_back(4ul);
  l.push_back("Happy!"s);
  l.push_back(42.5);

  EXPECT_EQ(l.front<unsigned long>(), 4ul);
  l.pop_front();
  EXPECT_EQ(l.front<std::string>(), "Happy!"s);
  l.pop_front();
  EXPECT_EQ(l.front<double>(), 42.5);
}

TEST(dlist, runtime_dlist)
{
  ryk::dlist l;
  ryk::RTA i(4);
  ryk::RTA d(43.20);
  ryk::RTA s("Happy!");
  
  l.push_back(i);
  l.push_back(d);
  l.push_back(s);

  std::cout << l << std::endl;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
