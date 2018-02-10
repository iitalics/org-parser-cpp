#include <cstdio>

void test_date();
void test_utils();
void test_header_parsing();
void test_body_parsing();
void test_file_parsing();
void test_serialize();

int main(void) {
  printf("testing org__private\n");
  test_utils();

  printf("testing org::LineParsing (header)\n");
  test_header_parsing();

  printf("testing org::LineParsing (body)\n");
  test_body_parsing();

  printf("testing org::parse_file\n");
  test_file_parsing();

  printf("testing org::serialize\n");
  test_serialize();

  printf("all tests passed.\n\n");
  return 0;
}
