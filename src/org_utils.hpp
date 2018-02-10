#pragma once
#include "org.hpp"
#include <cctype>
#include <cstring>

namespace org__private {

// removes the leftmost n chars from the given string
inline void mut_chompl(std::string *str, size_t n) {
  std::memmove(str->data(), str->data() + n, str->size() - n);
  str->resize(str->size() - n);
}

// removes the rightmost n chars from the given string
inline void mut_chompr(std::string *str, size_t n) {
  str->resize(str->size() - n);
}

// trim leading whitespace. returns the number of whitespace
// characters removed.
inline size_t mut_triml(std::string *str) {
  size_t i = 0;
  while (str->size() > i && std::isspace((*str)[i]))
    i++;
  mut_chompl(str, i);
  return i;
}

// trim trailing whitespace. returns the number of whitespace
// characters removed.
inline size_t mut_trimr(std::string *str) {
  size_t i = 0;
  while (str->size() > i && std::isspace((*str)[str->size() - i - 1]))
    i++;
  mut_chompr(str, i);
  return i;
}

/////////////////////////////////////////////////////

inline std::string trimlr(std::string str) {
  mut_trimr(&str);
  mut_triml(&str);
  return str;
}

inline void mut_chomp_triml(std::string *str, size_t n) {
  mut_chompl(str, n);
  mut_triml(str);
}
} // namespace org__private
