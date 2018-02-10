#include "../src/org_serialize.hpp"
#include <cassert>
#include <cstdio>

void test_serialize() {

  auto file = org::File();
  auto fst = org::Node(1, org::Header("First header", 5));
  fst.mut_tags()->insert("number-one");
  fst.mut_body()->push_back("hello");

  auto snd = org::Node(3, org::Header("Second header", 0, "TODO"));
  snd.mut_body()->push_back("world");
  snd.set_property("yes", "yas", 2);
  snd.set_property("no", "nah", 3);

  file.mut_nodes()->emplace_back(std::move(fst));
  file.mut_nodes()->emplace_back(std::move(snd));

  std::vector<std::string> lines;
  org::serialize(std::back_inserter(lines), file);

  assert(lines.size() == 6);
  assert(lines[0] == "* First header     :number-one:");
  assert(lines[1] == "  hello");
  assert(lines[2] == "*** TODO Second header");
  assert(lines[3] == "    :yes:  yas");
  assert(lines[4] == "    :no:   nah");
  assert(lines[5] == "    world");
}