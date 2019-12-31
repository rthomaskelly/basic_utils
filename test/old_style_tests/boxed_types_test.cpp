
#include <iostream>
#include <assert.h>

#include "boxed_types.hpp"
#include "traits.hpp"
#include "rank.hpp"

using std::cout;
using std::endl;
using namespace ryk;

int main() {
  constexpr Int i(-3);
  constexpr Nat n(4);
  
  static_assert(i == -3);
  static_assert(n == 4);
  static_assert(i + n == 1);

  assert(i == -3);
  assert(n == 4);

  static_assert(is_boxed_type_v<Int>);
  static_assert(is_boxed_type_v<Nat>);
  static_assert(!is_boxed_type_v<int>);
  static_assert(!is_boxed_type_v<Nat::boxedtype>);
  // cout << is_boxed_type_v<Int> << "\t(1)\n";
  // cout << is_boxed_type_v<Nat> << "\t(1)\n";
  // cout << is_boxed_type_v<int> << "\t(0)\n";
  // cout << is_boxed_type_v<Nat::boxedtype> << "\t(0)\n";
  // cout << std::is_scalar<Int>::value << "\t(0)\n";
  // cout << std::is_scalar<int>::value << "\t(1)\n";

  static_assert(rank_v<Int> == 0);
  static_assert(rank_v<int> == 0);
  static_assert(rank_v<Nat> == 0);
  static_assert(rank_v<Nat::boxed_type> == 0);
  
  static_assert(!std::is_arithmetic_v<Int>);
  static_assert(!std::is_arithmetic_v<Nat>);
  static_assert(std::is_arithmetic_v<Int::boxedtype>);
  static_assert(is_arithmetic_v<Int>);
  static_assert(is_arithmetic_v<Nat>);

  static_assert(!std::is_scalar<Int>::value);
  static_assert(!std::is_scalar_v<Int>);
  static_assert(std::is_scalar_v<Int::boxedtype>);
  static_assert(is_scalar_v<Int>);

  cout << "boxed type test success!\n";
  return 0;
}
