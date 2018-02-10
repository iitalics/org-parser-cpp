#include "../org.hpp"
#include <cassert>

void test_utils() {
  using namespace org__private;

  std::string hello("  hey world    ");

  auto hello1 = hello;
  mut_triml(&hello1);
  assert(hello1 == "hey world    ");

  auto hello2 = hello;
  mut_trimr(&hello2);
  assert(hello2 == "  hey world");

  assert(trimlr(std::move(hello)) == "hey world");
}
