#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <cstdint>

namespace org {

    ///
    /// \brief Class for representing dates by year/month/day.
    ///
    class date {
        uint32_t year_;
        uint8_t month_, day_;

    public:
        date(uint32_t y, uint8_t m, uint8_t d)
            : year_(y)
            , month_(m)
            , day_(d)
        {}

        date()
            : year_(0)
            , month_(0)
            , day_(0)
        {}

        uint32_t year() const { return year_; }
        uint32_t month() const { return month_; }
        uint32_t day() const { return day_; }

        bool is_empty() const {
            return year_ == 0
                && month_ == 0
                && day_ == 0;
        }

        std::optional<date> to_optional() const {
            return is_empty()
                ? std::optional<date>()
                : std::optional<date>(*this);
        }
    };

    ///
    /// \brief Enumeration for different priorities
    ///
    enum class priority {
        None = 0, A, B, C,
    };

    ///
    /// \brief Class for representing nodes in an Org Mode document
    ///
    class node {
    public:
        using tag_set = std::unordered_set<std::string>;
        using property_map = std::unordered_map<std::string, std::string>;

    private:
        size_t       level_;
        std::string  headline_, body_, tag_, todo_;
        priority     prty_;
        date         scheduled_, deadline_;
        tag_set      tags_;
        property_map props_;

    public:
        node(size_t lvl,
             std::string hl,
             std::string body,
             std::string tag,
             tag_set all_tags = tag_set())
            : level_(lvl)
            , headline_(std::move(hl))
            , body_(std::move(body))
            , tag_(std::move(tag))
            , prty_(priority::None)
            , tags_(std::move(all_tags))
        {}

        inline size_t level() const { return level_; }
        inline std::string const& headline() const { return headline_; }
        inline std::string const& tag() const { return tag_; }
        inline std::string const& todo() const { return todo_; }
        inline std::optional<date> scheduled() const { return scheduled_.to_optional(); }
        inline std::optional<date> deadline() const { return deadline_.to_optional(); }
    };

}
