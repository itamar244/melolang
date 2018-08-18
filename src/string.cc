#include "melo/string.h"
#include <cstring>
#include <iostream>

namespace melo {

String::String(Zone* zone, const std::string& str)
		: used_zone_(true)
		, size_(str.size()) {
	auto allocated_str = zone->NewArray<char>(str.size() + 1);
	std::strcpy(allocated_str, str.c_str());
	data_ = allocated_str;
}

} // namespace melo
