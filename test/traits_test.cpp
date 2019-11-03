
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <array>
#include <list>
#include <assert.h>

#include "traits.hpp"

using std::cout;
using std::endl;

using namespace ryk;

int main(int argc, char** argv)
{
  static_assert(is_insertable_v<std::vector<int>>);
  static_assert(is_insertable_v<std::set<int>>);
  static_assert(is_insertable_v<std::map<int, int>>);
  static_assert(is_insertable_v<std::string>);
  static_assert(is_insertable_v<std::list<int>>);
  static_assert(!is_insertable_v<std::stack<std::string>>);

  static_assert(std::is_same_v<subtype<std::vector<int>>, int>);
  static_assert(std::is_same_v<subtype<std::set<std::string>>, const std::string>);
  static_assert(std::is_same_v<subtype<std::map<int, int*>>, std::pair<const int, int*>>);

  static_assert(is_indexable_v<std::vector<int>>);
  static_assert(!is_indexable_v<std::set<int>>);
  static_assert(is_indexable_v<std::map<int, int>>);
  static_assert(is_indexable_v<std::string>);
  static_assert(!is_indexable_v<std::list<int>>);
  static_assert(!is_indexable_v<std::stack<std::string>>);


  cout << "traits test successful!\n";

  return 0;
}
