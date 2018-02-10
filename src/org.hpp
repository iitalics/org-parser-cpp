#pragma once
#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace org {

///
/// Class for headers in a node
///
class Header {
  std::string text_;
  size_t trailing_space_;
  std::string prefix_;

public:
  explicit Header(std::string text, size_t trailing_space = 0,
                  std::string prefix = "")
      : text_(text), trailing_space_(trailing_space), prefix_(prefix) {}

  std::string const &text() const { return text_; }
  size_t trailing_space() const { return trailing_space_; }
  std::string const &prefix() const { return prefix_; }
};

///
/// Properties in an Org Mode document
///
class Property {
  std::string key_, val_;
  size_t leading_space_;

public:
  Property(std::string k, std::string v, size_t spaces = 0)
      : key_(k), val_(v), leading_space_(spaces) {}

  std::string const &key() const { return key_; }
  std::string const &value() const { return val_; }
  size_t leading_space() const { return leading_space_; }
  std::string *mut_value() { return &val_; }
};

///
/// Nodes in an Org Mode document
///
class Node {
public:
  using TagSet = std::unordered_set<std::string>;
  using PropertyMap = std::vector<Property>;
  using Body = std::vector<std::string>;

private:
  size_t level_;
  Header header_;
  TagSet tags_;
  PropertyMap props_;
  std::vector<std::string> body_;

public:
  Node(size_t level, Header header)
      : level_(level), header_(std::move(header)) {}

  size_t level() const { return level_; }
  Header const &header() const { return header_; }
  TagSet const &tags() const { return tags_; }
  TagSet *mut_tags() { return &tags_; }
  PropertyMap const &properties() const { return props_; }
  Body const &body() const { return body_; }
  Body *mut_body() { return &body_; }

  void set_property(std::string const &key, std::string value,
                    size_t spaces = 0) {
    auto it = std::find_if(props_.begin(), props_.end(),
                           [&](Property &p) { return p.key() == key; });

    if (it == props_.end())
      props_.emplace_back(key, std::move(value), spaces);
    else
      std::swap(*it->mut_value(), value);
  }

  void add_property(Property prop) {
    set_property(prop.key(), std::move(*prop.mut_value()),
                 prop.leading_space());
  }

  std::optional<std::string> property(std::string const &key) const {
    auto it = std::find_if(props_.begin(), props_.end(),
                           [&](Property const &p) { return p.key() == key; });

    if (it == props_.end())
      return {};
    else
      return it->value();
  }
};

///
/// Class for a file full of nodes
///
class File {
  std::vector<Node> nodes_;

public:
  File() {}

  std::vector<Node> const &nodes() const { return nodes_; }
  std::vector<Node> *mut_nodes() { return &nodes_; }
};

///
/// A location in a file, for reporting in parse errors
///
class ParseLoc {
  size_t line_;

public:
  explicit ParseLoc(size_t line = 1) : line_(line) {}

  size_t line() const { return line_; }
  size_t *mut_line() { return &line_; }
};

///
/// Error to be raised during parsing
///
class ParseError : public std::runtime_error {

  static std::string build_message_(ParseLoc loc, const char *msg) {
    std::string full_msg = "parse error: line ";
    full_msg += std::to_string(loc.line());
    full_msg += ": ";
    full_msg += msg;
    return full_msg;
  }

  explicit ParseError(ParseLoc loc, const char *msg)
      : std::runtime_error(build_message_(loc, msg)) {}

public:
  static inline ParseError body_before_node(ParseLoc loc) {
    return ParseError(loc, "text found before first header");
  }
};
} // namespace org
