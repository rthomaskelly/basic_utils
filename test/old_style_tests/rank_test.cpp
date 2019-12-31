
#include <iostream>
#include <assert.h>
#include <vector>
#include <list>

#include "boxed_types.hpp"
#include "traits.hpp"
#include "wip/rank.hpp"

using std::cout;
using std::endl;
using namespace ryk;

int main() {
  static_assert(rank<int>() == 0);
  static_assert(rank<std::vector<int>>() == 1);

  std::vector<std::list<int>> vli;
  std::vector<std::vector<char>> vvc;
  static_assert(rank<std::vector<std::list<int>>>() == 2);
  static_assert(rank<decltype(vvc)>() == 2);
  
  subtype<std::vector<std::list<int>>> li;
  static_assert(rank<decltype(li)>() == 1);
  li.push_back(2);

  return 0;
}
