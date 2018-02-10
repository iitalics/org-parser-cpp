#include "../src/org_parser.hpp"
#include <cassert>
#include <cstdio>

void test_file_parsing() {

  {
    std::vector<std::string> lines = {
        "* First header     :1st:",
        "     just some",
        "",
        "     text      ",
        "** Second",
        "   :big:   true",
        "   :bad:   yes",
        "   :ugly:  perhaps",
    };

    auto file = org::parse_file(lines.begin(), lines.end());
    auto &fst = file.nodes()[0];
    auto &snd = file.nodes()[1];

    assert(fst.header().text() == "First header");
    assert(fst.tags().find("1st") != fst.tags().end());
    assert(fst.body().size() == 3);
    assert(fst.body().at(0) == "just some");
    assert(fst.body().at(1) == "");
    assert(fst.body().at(2) == "text");

    assert(snd.header().text() == "Second");
    assert(snd.properties().at("big") == "true");
    assert(snd.properties().at("bad") == "yes");
    assert(snd.properties().at("ugly") == "perhaps");
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
