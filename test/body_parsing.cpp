#include "../src/org_parser.hpp"
#include <cassert>
#include <cstdio>

void test_body_parsing() {
  {
    org::LineParser parse("hello world");
    if (parse.leading_property()) {
      assert(0);
    } else {
      assert(parse.line() == "hello world");
    }
  }

  {
    org::LineParser parse(":theprop: the val");
    if (auto key = parse.leading_property()) {
      assert(*key == "theprop");
      assert(parse.line() == "the val");
    } else {
      assert(0);
    }
  }

  {
    org::LineParser parse(":the:prop: the:val");
    if (auto key = parse.leading_property()) {
      assert(*key == "the:prop");
      assert(parse.line() == "the:val");
    } else {
      assert(0);
    }
  }

  {
    org::LineParser parse(":the :prop: the:val");
    if (auto key = parse.leading_property()) {
      assert(*key == "the :prop");
      assert(parse.line() == "the:val");
    } else {
      assert(0);
    }
  }

  {
    org::LineParser parse(":not:a:property");
    if (parse.leading_property()) {
      assert(0);
    } else {
      assert(parse.line() == ":not:a:property");
    }
  }

  {
    org::LineParser parse(": not a property");
    if (parse.leading_property()) {
      assert(0);
    } else {
      assert(parse.line() == ": not a property");
    }
  }
}
