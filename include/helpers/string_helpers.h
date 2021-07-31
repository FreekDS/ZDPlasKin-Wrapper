#ifndef ZDPLASKIN_INCLUDE_HELPERS_STRING_HELPERS_H_
#define ZDPLASKIN_INCLUDE_HELPERS_STRING_HELPERS_H_

#include <vector>
#include <string>

namespace utils {

std::vector<std::string> split(const std::string &string, const std::string &delimiter = " ");

void removeSubstr(std::string &source, const std::string &to_remove, unsigned int times = 0);

void removeWhitespaces(std::string &source);

std::vector<std::string> splitEqualSize(const std::string& string, unsigned int partSize);

} // namespace utils

#endif //ZDPLASKIN_INCLUDE_HELPERS_STRING_HELPERS_H_
