#pragma once

#include <cstdint>
#include <list>
#include <vector>

namespace melo {

struct Phrase {
	std::list<uint8_t> notes;
	float length;

	inline auto size() const { return notes.size(); }
};

}  // namespace melo
