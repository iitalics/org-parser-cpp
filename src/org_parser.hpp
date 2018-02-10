#pragma once
#include "org_utils.hpp"
#include <algorithm>
#include <utility>

namespace org {
    using namespace org__private;

    class LineParser {
        std::string line_;

        bool is_prefix_by(const char* s) const {
            for (size_t i = 0; s[i]; i++) {
                if (line_.size() <= i || line_[i] != s[i])
                    return false;
            }
            return true;
        }

    public:
        explicit LineParser(std::string ln)
            : line_(ln)
        {}

        std::string const& line() const { return line_; }

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
                mut_triml(&line_);
                return stars;
            } else {
                return {};
            }
        }

        /// if the line is preceded by TODO/DONE, parse that
        /// and returns the corresponding todo prefix.
        std::optional<Todo> header_todo() {
            if (is_prefix_by("TODO ")) {
                mut_chomp_triml(&line_, 4);
                return TODO;
            } else if (is_prefix_by("DONE ")) {
                mut_chomp_triml(&line_, 4);
                return DONE;
            } else {
                return {};
            }
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
                if (left == std::string::npos) break;

                // pull out tag substring
                auto tag = line_.substr(left + 1, right - left - 1);

                // dont use if it contains whitespace
                if (std::find_if(tag.cbegin(), tag.cend(),
                                 [] (char c) { return std::isspace(c); })
                    != tag.cend())
                    break;

                right = left;
                tags.push_back(tag);
            }

            line_.resize(right);
            mut_trimr(&line_);
            std::reverse(tags.begin(), tags.end());
            return tags;
        }

        // if this line is a header, parse it. returns a new node
        // with just the header
        std::optional<Node> node_header() {
            if (auto stars = header_level()) {
                // parse stuff
                std::optional<Priority> prio = {};
                std::optional<Todo> todo = header_todo();
                std::vector<std::string> tags = trailing_tags();

                // create the node; move tags into it
                auto node = Node(*stars, Header(std::move(line()), prio, todo));
                for (auto& tag : tags)
                    node.mut_tags()->emplace(std::move(tag));

                return node;
            } else {
                return {};
            }
        }


        // -------------------------------------------------
        // parsing body

        // if the line is a property, parses and returns the
        // key for the property.
        std::optional<std::string> leading_property() {
            if (line_.size() < 2 || line_.front() != ':')
                return {};

            size_t end = line_.find(": ", 1);
            if (end == std::string::npos)
                return {};

            auto key = line_.substr(1, end - 1);
            mut_chompl(&line_, end + 2);
            mut_triml(&line_);
            return key;
        }
    };


    // parse an entire file
    template <typename LineIterator>
    File parse_file(LineIterator lines_begin,
                    LineIterator lines_end) {

        ParseLoc loc;
        File file;
        std::optional<Node> current_node;

        for (LineIterator it = lines_begin;
             it != lines_end;
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
                if (parse.line().empty())
                    continue;

                // parse a property?
                if (auto prop_key = parse.leading_property()) {
                    current_node->mut_properties()
                        ->insert_or_assign(std::move(*prop_key),
                                           std::move(parse.line()));
                } else {
                    current_node->mut_body()
                        ->emplace_back(std::move(parse.line()));
                }
            }
        }

        // push the final node
        if (current_node.has_value())
            file.mut_nodes()->emplace_back(std::move(*current_node));

        return file;
    }
}
