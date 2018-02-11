#pragma once
#include "org.hpp"
#include <ostream>

namespace org {

template <typename CallbackFun>
void serialize_node(CallbackFun fun, Node const &node) {
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
  fun(tmp);

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
    fun(tmp);
  }

  // push body
  for (auto &line : node.body()) {
    if (line.empty()) {
      fun("");
    } else {
      tmp.clear();
      tmp += indent;
      tmp += line;
      fun(tmp);
    }
  }
}

template <typename CallbackFun>
void serialize(CallbackFun fun, File const &file) {
  for (auto &node : file.nodes())
    serialize_node(fun, node);
}

inline void serialize_to(std::ostream& os, File const &file) {
  serialize([&] (std::string const& line) { os << line << std::endl; }, file);
}

} // namespace org
