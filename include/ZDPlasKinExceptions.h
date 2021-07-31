#include <utility>

#ifndef ZDPLASKIN_INCLUDE_ZDPLASKINEXCEPTIONS_H_
#define ZDPLASKIN_INCLUDE_ZDPLASKINEXCEPTIONS_H_

class ZDPlaskinException : public std::exception {
 private:
	std::string _what;
 public:
	explicit ZDPlaskinException(std::string what) : _what(std::move(what)) {}
	[[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT override {
		return _what.c_str();
	}
};

class ParseException : public ZDPlaskinException {
 public:
	explicit ParseException(const std::string &what) : ZDPlaskinException(what) {}
};

class LibraryException : public ZDPlaskinException {
 public:
	explicit LibraryException(const std::string &what) : ZDPlaskinException(what) {}
};

class CompileException : public ZDPlaskinException {
 public:
	explicit CompileException(const std::string &what) : ZDPlaskinException(what) {}
};

class UtilsException : public ZDPlaskinException {
 public:
	explicit UtilsException(const std::string &what) : ZDPlaskinException(what) {}
};

#endif //ZDPLASKIN_INCLUDE_ZDPLASKINEXCEPTIONS_H_
