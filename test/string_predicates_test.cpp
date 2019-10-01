
#include <iostream>
#include <assert.h>

#include "string_predicates.hpp"

using namespace ryk;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
  std::string s0("test.1");
  std::string s1("01");
  std::string s2("00120");
  std::string s3("00.4a");

  static_assert(is_string_v<std::string>);
  static_assert(is_string_v<const std::string>);

  assert(!is_nat(s0));
  assert(is_nat(s1));
  assert(is_nat(s2));
  assert(!is_nat(s3));

  assert(!is_int(s0));
  assert(is_int(s1));
  assert(is_int(s2));
  assert(!is_int(s3));

  assert(!is_float(s0));
  assert(is_float(s1));
  assert(is_float(s2));
  assert(!is_float(s3));

  cout << "string predicates test successful!\n";

  return 0;
}
