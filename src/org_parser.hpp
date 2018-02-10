#pragma once
#include "org_utils.hpp"
#include <algorithm>

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
            if (line_.size() < 2 || line_.back() != ':') {
                return {};
            }

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

        // -------------------------------------------------
        // parsing body

        // if the line is prefixed with DEADLINE/SCHEDULED,
        // parses the prefix and returns the date. the prefix
        // is put into 'out_prefix'.
        std::optional<Date> date(std::string& out_prefix);

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
}
