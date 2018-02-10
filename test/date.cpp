#include "../src/org.hpp"
#include <cassert>
#include <iostream>

#define assert_eq(a, b) assert((a) == (b))

void test_date() {
  auto bday = org::Date(1997, 10, 10);
  assert(org::Date().is_empty());
  assert(org::Date(0, 0, 0).is_empty());
  assert(!bday.is_empty());
  assert_eq(org::Date().to_optional().has_value(), false);
  assert_eq(bday.to_optional().has_value(), true);
  assert_eq(bday.to_optional().value().year(), bday.year());
  assert_eq(bday.to_optional().value().month(), bday.month());
  assert_eq(bday.to_optional().value().day(), bday.day());
}
