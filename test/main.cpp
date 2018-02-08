#include <cstdio>

void test_date();
void test_utils();
void test_header_parsing();

int main(void)
{
    printf("testing org::Date\n");
    test_date();

    printf("testing org__private\n");
    test_utils();

    printf("testing org::LineParsing (header)\n");
    test_header_parsing();

    printf("all tests passed.\n\n");
    return 0;
}
