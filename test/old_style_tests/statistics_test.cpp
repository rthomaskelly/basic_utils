
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <array>
#include <list>
#include <assert.h>

#include "statistics.hpp"

using std::cout;
using std::endl;

using namespace ryk;

int main(int argc, char** argv)
{
  std::list<double> l{1, 2, 3, 4, 5, 6, 7};
  std::vector<int> v{1, 2, 3, 4, 5, 6, 7};
  // std::array<int, 7> a{1, 2, 3, 4, 5, 6, 7};
  
  // assert(sum(l) == 28); 
  assert(sum(v) == 28); 
  // assert(sum(a) == 28); 

  // assert(mean(l) == 4); 
  // assert(mean(v) == 4); 
  // assert(mean(a) == 4); 

  // assert(median(l) == 4); 
  // assert(median(v) == 4); 
  // assert(median(a) == 4); 

  cout << "statistics test successful!\n";

  return 0;
}
