#include <cstdio>
#include "../src/org.hpp"

void test_date();
void test_utils();

int main(void)
{
    printf("testing org::Date\n");
    test_date();

    printf("testing org__private\n");
    test_utils();

    printf("all tests passed.\n\n");
    return 0;
}
