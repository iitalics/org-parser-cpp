#include "../src/org_parser.hpp"
#include <cassert>
#include <cstdio>

void test_file_parsing() {

  {
    std::vector<std::string> lines = {
        "* First header              :1st:",
        "     just some",
        "",
        "     text      ",
        "** DONE Second",
        "   :big:   true",
        "   :bad:   yes",
        "   :ugly:  perhaps",
    };

    auto file = org::parse_file(lines.begin(), lines.end());
    auto &fst = file.nodes()[0];
    auto &snd = file.nodes()[1];

    assert(fst.header().text() == "First header");
    assert(fst.header().trailing_space() == 14);
    assert(fst.tags().find("1st") != fst.tags().end());
    assert(fst.body().size() == 3);
    assert(fst.body().at(0) == "just some");
    assert(fst.body().at(1) == "");
    assert(fst.body().at(2) == "text");

    assert(snd.header().text() == "Second");
    assert(snd.header().trailing_space() == 0);
    assert(snd.header().todo() == "DONE");
    assert(snd.property("big") == "true");
    assert(snd.property("bad") == "yes");
    assert(snd.property("ugly") == "perhaps");
  }

  {
    std::vector<std::string> lines{"yikes"};
    try {
      org::parse_file(lines.begin(), lines.end());
      assert(0);
    } catch (org::ParseError &e) {
      // OK
    }
  }
}
