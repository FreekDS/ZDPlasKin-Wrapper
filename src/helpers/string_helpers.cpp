#include "helpers/string_helpers.h"
#include <algorithm>

namespace utils {

std::vector<std::string> split(const std::string &string, const std::string &delimiter) {
	size_t start = 0;
	size_t end = string.find(delimiter);
	std::vector<std::string> result;
	while (end != std::string::npos) {
		result.push_back(string.substr(start, end - start));
		start = end + delimiter.length();
		end = string.find(delimiter, start);
	}
	return result;
}

void removeSubstr(std::string &source, const std::string &to_remove, unsigned int times) {
	size_t pos;
	unsigned int count = 0;
	while ((pos = source.find(to_remove)) != std::string::npos) {
		if (times > 0 && count >= times)
			return;
		source.erase(pos, to_remove.length());
		count++;
	}
}

void removeWhitespaces(std::string &source) {
	source.erase(std::remove_if(source.begin(), source.end(), ::isspace), source.end());
}

} // namespace utils