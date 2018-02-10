#include "../src/org_parser.hpp"
#include <cassert>
#include <cstdio>

void test_body_parsing() {
  {
    org::LineParser parse("hello world");
    if (parse.property()) {
      assert(0);
    } else {
      assert(parse.line() == "hello world");
    }
  }

  {
    org::LineParser parse(":theprop: the val");
    if (auto prop = parse.property()) {
      assert(prop->key() == "theprop");
      assert(prop->value() == "the val");
      assert(prop->leading_space() == 1);
    } else {
      assert(0);
    }
  }

  {
    org::LineParser parse(":the:prop:     the:val");
    if (auto prop = parse.property()) {
      assert(prop->key() == "the:prop");
      assert(prop->value() == "the:val");
      assert(prop->leading_space() == 5);
    } else {
      assert(0);
    }
  }

  {
    org::LineParser parse(":the :prop: the:val");
    if (auto prop = parse.property()) {
      assert(prop->key() == "the :prop");
      assert(prop->value() == "the:val");
    } else {
      assert(0);
    }
  }

  {
    org::LineParser parse(":not:a:property");
    if (parse.property()) {
      assert(0);
    } else {
      assert(parse.line() == ":not:a:property");
    }
  }

  {
    org::LineParser parse(": not a property");
    if (parse.property()) {
      assert(0);
    } else {
      assert(parse.line() == ": not a property");
    }
  }
}
