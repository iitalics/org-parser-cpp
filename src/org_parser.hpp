#pragma once
#include "org_utils.hpp"

namespace org {
    using namespace org__private;

    class LineParser {
        std::string line_;

        bool prefix_by(const char* s) const {
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
            if (prefix_by("*")) {
                size_t stars = 0;
                while (stars < line_.size() && line_[stars] == '*')
                    stars++;

                mut_chompl(&line_, stars);
                mut_triml(&line_);
                return std::optional<int>(stars);
            } else {
                return std::optional<int>();
            }
        }

        /// if the line is preceded by TODO/DONE, parse that
        /// and returns the corresponding todo prefix.
        std::optional<Todo> header_todo() {
            if (prefix_by("TODO ")) {
                mut_chomp_triml(&line_, 4);
                return std::optional<Todo>(TODO);
            } else if (prefix_by("DONE ")) {
                mut_chomp_triml(&line_, 4);
                return std::optional<Todo>(DONE);
            } else {
                return std::optional<Todo>();
            }
        }

        /// parses trailing tags and returns them all.
        std::vector<std::string> trailing_tags();

        // -------------------------------------------------
        // parsing body

        // if the line is prefixed with DEADLINE/SCHEDULED,
        // parses the prefix and returns the date. the prefix
        // is put into 'out_prefix'.
        std::optional<Date> date(std::string& out_prefix);

        // if the line is a property, parses and returns the
        // key for the property.
        std::optional<std::string> leading_property();
    };
}
