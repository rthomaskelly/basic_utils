#include "../string_predicates.hpp"
#include <iostream>

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

  cout << is_nat(s0) << "\t(0)\n";
  cout << is_nat(s1) << "\t(1)\n";
  cout << is_nat(s2) << "\t(1)\n";
  cout << is_nat(s3) << "\t(0)\n";

  cout << is_int(s0) << "\t(0)\n";
  cout << is_int(s1) << "\t(1)\n";
  cout << is_int(s2) << "\t(1)\n";
  cout << is_int(s3) << "\t(0)\n";

  cout << is_float(s0) << "\t(0)\n";
  cout << is_float(s1) << "\t(1)\n";
  cout << is_float(s2) << "\t(1)\n";
  cout << is_float(s3) << "\t(0)\n";


  return 0;
}
