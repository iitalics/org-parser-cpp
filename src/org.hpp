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
        Header(std::string t)
            : text_(t)
        {}

        std::optional<Priority> priority() const { return prio_; }
        std::optional<Todo> todo() const { return todo_; }
        void clear_priority() { prio_.reset(); }
        void clear_todo() { todo_.reset(); }

        Priority* mut_priority() {
            if (!prio_.has_value())
                prio_.emplace('A');
            return &*prio_;
        }

        Todo* mut_todo() {
            if (!todo_.has_value())
                todo_.emplace(true);
            return &*todo_;
        }
    };

    ///
    /// Class for nodes in an Org Mode document
    ///
    class Node {
    public:
        using TagSet = std::unordered_set<std::string>;
        using PropertyMap = std::unordered_map<std::string, std::string>;

    private:
        size_t      level_;
        Header      header_;
        Date        scheduled_, deadline_;
        TagSet      tags_;
        PropertyMap props_;

    public:
        size_t level() const { return level_; }
        Header const& header() const { return header_; }
        Date scheduled() const { return scheduled_; }
        Date deadline() const { return deadline_; }
        TagSet const& tags() const { return tags_; }
        TagSet* mut_tags() { return &tags_; }
    };


    ///
    /// Class for a file full of nodes
    ///
    class File {
        std::vector<Node> nodes_;
    public:
        File();
        std::vector<Node> const& nodes() const { return nodes_; }
        std::vector<Node>* mut_nodes() { return &nodes_; }
    };


    ///
    /// Error to be raised during parsing
    ///
    class ParseError : public std::runtime_error {

        static std::string build_message_(const char* msg)
        {
            std::string full_msg = "Error parsing: ";
            full_msg += msg;
            return full_msg;
        }

        explicit ParseError(const char* msg)
            : std::runtime_error(build_message_(msg))
        {}

    public:
        // TODO: static constructor methods
    };
}
