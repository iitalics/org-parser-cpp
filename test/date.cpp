#include <iostream>
#include <cassert>
#include "../src/org.hpp"

void test_date()
{
    assert(org::date().is_empty());
    assert(org::date(0, 0, 0).is_empty());
    assert(!org::date(2017, 5, 4).is_empty());
}
