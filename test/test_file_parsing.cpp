#include "../org.hpp"
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
    assert(fst.header().prefix().empty());
    assert(fst.tags().size() == 1);
    assert(fst.has_tag("1st"));
    assert(fst.body().size() == 3);
    assert(fst.body().at(0) == "just some");
    assert(fst.body().at(1) == "");
    assert(fst.body().at(2) == "text");

    assert(snd.header().text() == "Second");
    assert(snd.header().trailing_space() == 0);
    assert(snd.header().prefix() == "DONE");
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
      assert(std::string(e.what()) == "parse error: line 1: "
                                      "text found before first header");
    }
  }

  {
    std::vector<std::string> lines = {
        "* Header",
        "  :p: 1",
        "  :p: 2",
    };
    try {
      org::parse_file(lines.begin(), lines.end());
      assert(0);
    } catch (org::ParseError &e) {
      assert(std::string(e.what()) ==
             "parse error: line 3: "
             "multiple occurrences of the same property");
    }
  }
}
