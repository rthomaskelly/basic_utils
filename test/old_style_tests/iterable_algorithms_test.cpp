
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <array>
#include <list>
#include <assert.h>

#include "iterable_algorithms.hpp"

using std::cout;
using std::endl;

using namespace ryk;

int main(int argc, char** argv)
{

  std::vector<int> v{1, 3, 3, 2, 5, 9, 3, 2, 5, 1};
  
  auto it = find(v, 2);
  assert(*it == 2);
  it = find_if(v, [](auto i){ return i == 9; });
  assert(*it == 9);
  auto index = index_of(v, 2);
  assert(index == 3);
  assert(sum(v) == 34);

  std::vector<double> vd;
  transform(v, std::back_inserter(vd), [](int i){ return static_cast<double>(i); }); 
  assert(mean(v) == 3);
  assert(sum(vd) == 34.0);
  assert(mean(vd) == 3.4);

  auto vc = v;
  erase_duplicates(sort(vc));
  auto vi = std::vector<int>{1, 2, 3, 5, 9};
  assert(vc == vi); 

  assert(has(vc, 3));
  assert(!has(vc, 4)); 
  

  cout << "iterable algorithms test successful!\n";

  return 0;
}
