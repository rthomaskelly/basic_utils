
#include <iostream>

#include "sparse_square_matrix.hpp"

using namespace ryk;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
  sparse_square_matrix<int, int> ssm;
  ssm.insert(0, 1, 2);
 
  cout << ssm << endl; 
  
  return 0;
}
