#include "../src/org_parser.hpp"
#include <cassert>

void test_header_parsing() {
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
