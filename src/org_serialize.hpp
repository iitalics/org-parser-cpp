#pragma once
#include "org.hpp"

namespace org {

template <typename BackInsertLine>
BackInsertLine serialize_node(BackInsertLine out, Node const &node) {
  std::string tmp;

  // ------------------------
  // HEADER:

  // insert *'s and prefix
  tmp += std::string(node.level(), '*');
  tmp += ' ';
  if (!node.header().prefix().empty()) {
    tmp += node.header().prefix();
    tmp += ' ';
  }

  // insert text and trailing space
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
    tmp += ':';
    tmp += std::string(prop.leading_space(), ' ');
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
  return out;
}

template <typename BackInsertLine>
BackInsertLine serialize(BackInsertLine out, File const &file) {
  for (auto &node : file.nodes())
    out = serialize_node(out, node);
  return out;
}
} // namespace org
