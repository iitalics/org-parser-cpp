#pragma once
#include "org.hpp"
#include <cctype>
#include <cstring>

namespace org__private {

    // trim leading whitespace
    inline void mut_triml(std::string* str) {
        size_t i = 0;
        while (str->size() > i && std::isspace((*str)[i]))
            i++;

        std::memmove(str->data(), str->data() + i, str->size() - i);
        str->resize(str->size() - i);
    }

    // trim trailing whitespace
    inline void mut_trimr(std::string* str) {
        size_t i = 0;
        while (str->size() > i && std::isspace((*str)[str->size() - i -1]))
            i++;

        str->resize(str->size() - i);
    }

    inline std::string trimlr(std::string str) {
        mut_trimr(&str);
        mut_triml(&str);
        return str;
    }
}
