#pragma once
#include "org_utils.hpp"
#include <algorithm>
#include <utility>

namespace org {
using namespace org__private;

class LineParser {
  std::string line_;

  bool is_prefix_by(const char *s) const {
    for (size_t i = 0; s[i]; i++) {
      if (line_.size() <= i || line_[i] != s[i])
        return false;
    }
    return true;
  }

public:
  explicit LineParser(std::string ln) : line_(ln) {}

  std::string const &line() const { return line_; }

  // trim leading and trailing whitespace
  void trim() {
    mut_trimr(&line_);
    mut_triml(&line_);
  }

  // -------------------------------------------------
  // parsing header

  /// if line is preceded by asterisks, parses them
  /// and then returns the number of asterisks.
  std::optional<size_t> header_level() {
    if (is_prefix_by("*")) {
      size_t stars = 0;
      while (stars < line_.size() && line_[stars] == '*')
        stars++;

      mut_chompl(&line_, stars);
      return stars;
    } else {
      return {};
    }
  }

  /// if the line is preceded by ALL CAPS, parse that
  /// and returns the corresponding todo prefix. trims
  /// leading whitespace no matter what.
  std::optional<Todo> header_todo() {
    mut_triml(&line_);

    size_t space = line_.find(' ');
    if (space == std::string::npos)
      return {};

    // check that its ALL CAPS
    if (!std::all_of(line_.cbegin(), line_.cbegin() + space,
                     [](char c) { return 'A' <= c && c <= 'Z'; }))
      return {};

    // extract and remove substring
    auto todo = line_.substr(0, space);
    mut_chomp_triml(&line_, space);
    return todo;
  }

  /// parses trailing tags and returns them all.
  std::vector<std::string> trailing_tags() {
    if (line_.size() < 2 || line_.back() != ':')
      return {};

    std::vector<std::string> tags;

    // rightmost ':'
    size_t right = line_.size() - 1;
    for (;;) {
      // next ':' to the left of rightmost
      size_t left = line_.rfind(':', right - 1);
      if (left == std::string::npos)
        break;

      // pull out tag substring
      auto tag = line_.substr(left + 1, right - left - 1);

      // dont use if it contains whitespace
      if (std::any_of(tag.cbegin(), tag.cend(),
                      [](char c) { return std::isspace(c); }))
        break;

      right = left;
      tags.push_back(tag);
    }

    line_.resize(right);
    std::reverse(tags.begin(), tags.end());
    return tags;
  }

  // if this line is a header, parse it. returns a new node
  // with just the header.
  std::optional<Node> node_header() {
    if (auto stars = header_level()) {
      // parse stuff
      std::optional<Priority> prio = {};
      std::optional<Todo> todo = header_todo();
      std::vector<std::string> tags = trailing_tags();
      size_t trailing = mut_trimr(&line_);

      // create the node; move tags into it
      auto node = Node(*stars, Header(std::move(line_), trailing, prio, todo));
      for (auto &tag : tags)
        node.mut_tags()->emplace(std::move(tag));

      return node;
    } else {
      return {};
    }
  }

  // -------------------------------------------------
  // parsing body

  // if the line is a property, parses and returns the
  // key and value as a Property.
  std::optional<Property> property() {
    if (line_.size() < 2 || line_.front() != ':')
      return {};

    size_t end = line_.find(": ", 1);
    if (end == std::string::npos)
      return {};

    // 1 because we need to skip the initial :
    auto key = line_.substr(1, end - 1);
    // + 1 because we need to skip the final :
    mut_chompl(&line_, end + 1);
    size_t spaces = mut_triml(&line_);

    return Property(key, std::move(line_), spaces);
  }
};

// parse an entire file
template <typename LineIterator>
File parse_file(LineIterator lines_begin, LineIterator lines_end) {

  ParseLoc loc;
  File file;
  std::optional<Node> current_node;

  for (LineIterator it = lines_begin; it != lines_end;
       ++it, (*loc.mut_line())++) {

    auto parse = LineParser(*it);

    // parse a header?
    if (auto new_node = parse.node_header()) {

      // push previous node
      if (current_node.has_value())
        file.mut_nodes()->emplace_back(std::move(*current_node));

      // set up new node
      current_node.emplace(std::move(*new_node));

    } else {
      if (!current_node.has_value())
        throw ParseError::body_before_node(loc);

      parse.trim();

      // parse a property?
      if (auto prop = parse.property()) {
        current_node->add_property(std::move(*prop));
      } else {
        current_node->mut_body()->emplace_back(std::move(parse.line()));
      }
    }
  }

  // push the final node
  if (current_node.has_value())
    file.mut_nodes()->emplace_back(std::move(*current_node));

  return file;
}
} // namespace org
