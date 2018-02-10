#include "../src/org_parser.hpp"
#include <cassert>
#include <cstdio>

static void test_header_stars() {
  {
    org::LineParser parse("***  Header at level 3");
    if (auto stars = parse.header_level()) {
      assert(*stars == 3);
      assert(parse.line() == "  Header at level 3");
    } else {
      assert(0);
    }
  }

  {
    org::LineParser parse("* TODOMSG   Header at level 1");
    if (auto stars = parse.header_level()) {
      assert(*stars == 1);

      if (auto todo = parse.header_todo()) {
        assert(*todo == "TODOMSG");
      } else {
        assert(0);
      }

      assert(parse.line() == "Header at level 1");
    } else {
      assert(0);
    }
  }

  {
    org::LineParser parse("  * not a header");
    if (auto stars = parse.header_level()) {
      assert(0);
    }
    assert(parse.line() == "  * not a header");
  }
}

static void test_trailing_tags() {
  {
    org::LineParser parse("header    :t1:tag2:");
    auto tags = parse.trailing_tags();
    assert(tags.size() == 2);
    assert(tags[0] == "t1");
    assert(tags[1] == "tag2");
    assert(parse.line() == "header    ");
  }

  {
    org::LineParser parse(" the header    :");
    auto tags = parse.trailing_tags();
    assert(tags.size() == 0);
    assert(parse.line() == " the header    ");
  }

  {
    org::LineParser parse("no tags ::: here");
    auto tags = parse.trailing_tags();
    assert(tags.size() == 0);
    assert(parse.line() == "no tags ::: here");
  }

  {
    org::LineParser parse("tags :cant have :spaces:");
    auto tags = parse.trailing_tags();
    assert(tags.size() == 1);
    assert(tags[0] == "spaces");
    assert(parse.line() == "tags :cant have ");
  }
}

void test_header_parsing() {
  printf("  LineParser::header_stars, header_todo\n");
  test_header_stars();
  printf("  LineParser::tailing_tags\n");
  test_trailing_tags();

  printf("  LineParser::header\n");
  {
    org::LineParser parse("**** TODO Some: header    :x:yz:");
    if (auto node = parse.node_header()) {
      assert(node->header().text() == "Some: header");
      assert(node->header().trailing_space() == 4);
      assert(node->header().priority() == std::nullopt);
      assert(node->header().todo() == "TODO");

      assert(node->tags().size() == 2);
      assert(node->tags().find("x") != node->tags().end());
      assert(node->tags().find("yz") != node->tags().end());

    } else {
      assert(0);
    }
  }
}
