#include <iostream>
#include <cassert>
#include "../src/org.hpp"

#define assert_eq(a,b) assert((a)==(b))

void test_date()
{
    auto bday = org::date(1997, 10, 10);
    assert(org::date().is_empty());
    assert(org::date(0, 0, 0).is_empty());
    assert(!bday.is_empty());
    assert_eq(org::date().to_optional().has_value(), false);
    assert_eq(bday.to_optional().has_value(), true);
    assert_eq(bday.to_optional().value().year(), bday.year());
    assert_eq(bday.to_optional().value().month(), bday.month());
    assert_eq(bday.to_optional().value().day(), bday.day());
}
