#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <cstdint>

namespace org {

    ///
    /// \brief Class for representing dates by year/month/day.
    ///
    class Date {
        uint32_t year_;
        uint8_t month_, day_;

    public:
        Date(uint32_t y, uint8_t m, uint8_t d)
            : year_(y)
            , month_(m)
            , day_(d)
        {}

        Date() : Date(0, 0, 0) {}

        uint32_t year() const { return year_; }
        uint32_t month() const { return month_; }
        uint32_t day() const { return day_; }

        bool is_empty() const {
            return year_ == 0
                && month_ == 0
                && day_ == 0;
        }

        std::optional<Date> to_optional() const {
            if (is_empty())
                return {};
            else
                return *this;
        }
    };

    ///
    /// A priority is one of 'A', 'B', or 'C'
    ///
    using Priority = char;

    ///
    /// A Todo is either TODO or DONE
    ///
    using Todo = bool;
    constexpr bool TODO = true;
    constexpr bool DONE = false;


    ///
    /// Class for headers in a node
    ///
    class Header {
        std::string text_;
        std::optional<Priority> prio_;
        std::optional<Todo> todo_;

    public:
        explicit Header(std::string text,
                        std::optional<Priority> prio = {},
                        std::optional<Todo> todo = {})
            : text_(text)
            , prio_(prio)
            , todo_(todo)
        {}

        std::string const& text() const { return text_; }
        std::optional<Priority> priority() const { return prio_; }
        std::optional<Todo> todo() const { return todo_; }
    };


    ///
    /// Class for nodes in an Org Mode document
    ///
    class Node {
    public:
        using TagSet = std::unordered_set<std::string>;
        using PropertyMap = std::unordered_map<std::string, std::string>;
        using Body = std::vector<std::string>;

    private:
        size_t      level_;
        Header      header_;
        TagSet      tags_;
        PropertyMap props_;
        std::vector<std::string> body_;

    public:
        Node(size_t level, Header header)
            : level_(level)
            , header_(std::move(header))
        {}

        size_t level() const { return level_; }
        Header const& header() const { return header_; }
        TagSet const& tags() const { return tags_; }
        TagSet* mut_tags() { return &tags_; }
        PropertyMap const& properties() const { return props_; }
        PropertyMap* mut_properties() { return &props_; }
        Body const& body() const { return body_; }
        Body* mut_body() { return &body_; }

        std::optional<std::string> property(std::string const& key) const {
            auto it = props_.find(key);
            if (it != props_.end())
                return it->second;
            else
                return {};
        }
    };


    ///
    /// Class for a file full of nodes
    ///
    class File {
        std::vector<Node> nodes_;
    public:
        File() {}

        std::vector<Node> const& nodes() const { return nodes_; }
        std::vector<Node>* mut_nodes() { return &nodes_; }
    };



    ///
    /// A location in a file, for reporting in parse errors
    ///
    class ParseLoc {
        size_t line_;
    public:
        explicit ParseLoc(size_t line = 1)
            : line_(line)
        {}

        size_t line() const { return line_; }
        size_t* mut_line() { return &line_; }
    };

    ///
    /// Error to be raised during parsing
    ///
    class ParseError : public std::runtime_error {

        static std::string build_message_(ParseLoc loc, const char* msg) {
            std::string full_msg = "parse error: line ";
            full_msg += std::to_string(loc.line());
            full_msg += ": ";
            full_msg += msg;
            return full_msg;
        }

        explicit ParseError(ParseLoc loc, const char* msg)
            : std::runtime_error(build_message_(loc, msg))
        {}

    public:

        static inline ParseError body_before_node(ParseLoc loc) {
            return ParseError(loc, "text found before first header");
        }

    };
}
