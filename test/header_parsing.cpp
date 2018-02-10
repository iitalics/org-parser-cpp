#include "../src/org_parser.hpp"
#include <cassert>
#include <cstdio>

static void test_header_stars() {
    {
        org::LineParser parse("***  Header at level 3");
        if (auto stars = parse.header_level()) {
            assert(*stars == 3);
            assert(parse.line() == "Header at level 3");
        } else {
            assert(0);
        }
    }

    {
        org::LineParser parse("* TODO  Header at level 1");
        if (auto stars = parse.header_level()) {
            assert(*stars == 1);

            if (auto todo = parse.header_todo()) {
                assert(*todo == org::TODO);
            } else {
                assert(0);
            }

            assert(parse.line() == "Header at level 1");
        } else {
            assert(0);
        }
    }

    {
        org::LineParser parse("  * not a header");
        if (auto stars = parse.header_level()) {
            assert(0);
        }
        assert(parse.line() == "  * not a header");
    }
}

static void test_trailing_tags() {
    {
        org::LineParser parse("header    :t1:tag2:");
        auto tags = parse.trailing_tags();
        assert(tags.size() == 2);
        assert(tags[0] == "t1");
        assert(tags[1] == "tag2");
        assert(parse.line() == "header");
    }

    {
        org::LineParser parse(" the header    :");
        auto tags = parse.trailing_tags();
        assert(tags.size() == 0);
        assert(parse.line() == " the header");
    }

    {
        org::LineParser parse("no tags ::: here");
        auto tags = parse.trailing_tags();
        assert(tags.size() == 0);
        assert(parse.line() == "no tags ::: here");
    }

    {
        org::LineParser parse("tags :cant have :spaces:");
        auto tags = parse.trailing_tags();
        assert(tags.size() == 1);
        assert(tags[0] == "spaces");
        assert(parse.line() == "tags :cant have");
    }
}

void test_header_parsing() {
    printf("  LineParser::header_stars, header_todo\n");
    test_header_stars();
    printf("  LineParser::tailing_tags\n");
    test_trailing_tags();

    printf("  LineParser::header\n");
    {
        org::LineParser parse("**** TODO Some: header    :x:yz:");
        if (auto header_tags = parse.header()) {
            auto head = std::move(header_tags->first);
            auto tags = std::move(header_tags->second);

            assert(head.text() == "Some: header");
            assert(head.priority() == std::nullopt);
            assert(head.todo() == org::TODO);

            assert(tags.size() == 2);
            assert(tags.find("x") != tags.end());
            assert(tags.find("yz") != tags.end());

        } else {
            assert(0);
        }
    }
}
