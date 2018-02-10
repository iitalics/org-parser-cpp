#pragma once
#include "org.hpp"

namespace org {

template <typename BackInsertLine>
void serialize(BackInsertLine out, File const &file) {

  std::string tmp;

  for (auto &node : file.nodes()) {
    tmp.clear();

    // ------------------------
    // HEADER:

    // insert *'s, name and trailing spaces
    tmp += std::string(node.level(), '*');
    tmp += ' ';
    tmp += node.header().text();
    tmp += std::string(node.header().trailing_space(), ' ');

    // insert tags:
    if (!node.tags().empty()) {
      tmp += ':';
      for (auto &tag : node.tags()) {
        tmp += tag;
        tmp += ':';
      }
    }

    // push header line
    *out++ = tmp;

    // ------------------------
    // BODY:

    auto indent = std::string(1 + node.level(), ' ');

    // push properties
    for (auto &prop : node.properties()) {
      tmp.clear();
      tmp += indent;
      tmp += ':';
      tmp += prop.key();
      tmp += ": ";
      tmp += prop.value();
      *out++ = tmp;
    }

    // push body
    for (auto &line : node.body()) {
      if (line.empty()) {
        *out++ = "";
      } else {
        tmp.clear();
        tmp += indent;
        tmp += line;
        *out++ = tmp;
      }
    }
  }
}
} // namespace org
