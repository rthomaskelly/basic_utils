
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <array>
#include <list>
#include <assert.h>

#include "graph.hpp"

using std::cout;
using std::endl;

using namespace ryk;

int main(int argc, char** argv)
{
  auto g = directed_graph<int, int>{0};
  g.add_child(0, 1);
  g.add_child(0, 2);
  g.add_child(0, 3);

  g.add_child(1, 10);
  g.add_child(1, 11);
  g.add_child(11, 110);
  g.add_child(110, 1100);

  g.add_child(10, 1100);
  g.add_child(2, 20);
  g.add_child(2, 21);
  g.add_child(20, 200);
  g.add_child(21, 210);
  g.add_child(21, 211);
   

  cout << g << endl;

  cout << "10's children: ";
  for (auto& c : g.children(10)) cout << c.first << " ";
  cout << endl;
  cout << "\nTargeting 20:\n";
  auto touch_dbg = [](auto n){ cout << "touch '" << n.first << "'\n"; };
  auto search_dbg = [](auto n){ cout << "search '" << n.first << "\n"; };
  auto child_dbg = [](auto child, auto parent){ cout << "onchild '" << child.first << "'\n"; };
  cout << g.targeted_depth_search(20, touch_dbg, search_dbg, child_dbg);
  cout << "\nRoot seed search:\n";
  g.seeded_depth_search([](auto n){ if (n.first == 10) cout << "touch  '" << n.first << "'\n"; },
                        [](auto n){ if (n.first == 10) cout << "search '" << n.first << "'\n"; },
                        [](auto child, auto parent){ 
                          if (parent.first == 10) cout << "child '" << parent.first << "'\n";});

  cout << "\nSeed depth search from 10:\n";
  g.seeded_depth_search(10, touch_dbg, search_dbg, child_dbg);

  cout << "\n1100 from 10? " << g.depth_search(10, 1100) << endl << endl;
 
  const auto gc = g;
  cout << gc << endl;
  
  //g.full_search<std::stack>(1, [](auto n){ cout << "touched '" << n << "'\n"; },
  //              [](auto n){ cout << "searched '" << n << "'\n"; });

  cout << "\ngraph test successful!\n";

  return 0;
}
