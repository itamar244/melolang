#pragma once

#include <cstdint>
#include <list>
#include <utility>

namespace melo {

struct Phrase {
	std::list<uint8_t> notes;
	float length;

	inline Phrase() : notes({}), length(0) {}

	inline Phrase(std::list<uint8_t>&& notes, float length)
			: notes(std::move(notes)), length(length) {}

	inline Phrase& operator=(Phrase&& rhs) {
		notes = std::move(rhs.notes);
		length = rhs.length;
		return *this;
	}
};

inline uint16_t PhraseLengthInMillis(const Phrase& phrase, float speed) {
	return 60 * phrase.length / speed;
}

}  // namespace melo
