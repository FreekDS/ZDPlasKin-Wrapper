#include "helpers/string_helpers.h"
#include "ZDPlasKinExceptions.h"

#include <iostream>

namespace utils {

std::vector<std::string> split(const std::string &string, const std::string &delimiter) {
	size_t start = 0, end;
	std::vector<std::string> result{};
	while ((end = string.find(delimiter, start)) != std::string::npos) {
		result.push_back(string.substr(start, end - start));
		start = end + delimiter.length();
	}
	result.push_back(string.substr(start, string.length() - start));
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

std::vector<std::string> splitEqualSize(const std::string &string, const unsigned int partSize) {
	if (string.length()%partSize != 0) {
		throw UtilsException("String '" + string + "' has no size divisible by " + std::to_string(partSize)
								 + ": cannot split it in equal parts");
	}
	const unsigned int count = string.length() / partSize;
	std::vector<std::string> result(count);
	unsigned int s_i = 0;
	for(int i = 0; i<count; i++) {
		result[i] = string.substr(s_i, partSize);
		s_i += partSize;
	}
	return result;
}

} // namespace utils