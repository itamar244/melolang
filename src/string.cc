#include "melo/string.h"
#include <cstring>
#include <iostream>

namespace melo {

static const char* CopyStr(const char* str, std::size_t size) {
	auto val = new char[size + 1];
	std::strcpy(val, str);
	return val;
}

// String::String(const char*& buffer)
// 		: used_zone_(false)
// 		, size_(std::strlen(buffer))
// 		, data_(CopyStr(buffer, size_)) {}

String::String(const std::string& str)
		: size_(str.size())
		, data_(CopyStr(str.c_str(), size_)) {}

String::String(const String& str)
		: size_(str.size_)
		, data_(CopyStr(str.data_, size_)) {}

String::String(String&& str)
		: size_(str.size_), data_(str.data_) {
	str.data_ = nullptr;
	str.size_ = 0;
}

String::~String() {
	if (data_) delete[] data_;
}

// String::String(const String& copy)
// 		: used_zone_(copy.used_zone_)
// 		, size_(copy.size_) {
// 	if (used_zone_) {
// 		data_ = copy.data_;
// 	} else {
// 		data_ = CopyStr(copy.data_, size_);
// 	}
// }

// static char* Concat(
// 		const char* lhs, std::size_t lhs_size,
// 		const char* rhs, std::size_t rhs_size) {
// 	char* cat = new char[lhs_size + rhs_size + 1];
//
// 	std::strncpy(cat, lhs, lhs_size);
// 	std::strncpy(cat + lhs_size, rhs, rhs_size);
// 	cat[lhs_size + rhs_size] = '\0';
// 	return cat;
// }
//
// String String::operator+(const String& rhs) const {
// 	String next;
//
// 	next.data_ = Concat(data_, size_, rhs.data_, rhs.size_);
// 	next.size_ = size_ + rhs.size_;
// 	return next;
// }
//
// String String::operator+(const char* rhs) const {
// 	String next;
// 	std::size_t rhs_size = std::strlen(rhs);
//
// 	next.data_ = Concat(data_, size_, rhs, rhs_size);
// 	next.size_ = size_ + rhs_size;
// 	return next;
// }
//
// String operator+(const char* lhs, const String& rhs) {
// 	String next;
// 	std::size_t lhs_size = std::strlen(lhs);
//
// 	next.data_ = Concat(lhs, lhs_size, rhs.data_, rhs.size_);
// 	next.size_ = lhs_size + rhs.size_;
// 	return next;
// }

} // namespace melo
