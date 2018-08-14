#include "evaluator/utils.h"
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <atic/iterables.h>

namespace melo::evaluator {

namespace {

const std::map<char, uint8_t> kNotesToInt({
	{'C', 0}, {'D', 2}, {'E', 4}, {'F', 5}, {'G', 7}, {'A', 9}, {'B', 11},
});

inline float LengthToFloat(const ast::NumericLiteralPtr& length) {
	auto size = length->value.size();
	bool plus_half = *length->value.crbegin() == '.';

	if (plus_half) --size;
	auto buffer = length->value.substr(0, size).c_str();
	float length_value = 32 / float(std::atoi(buffer));

	return plus_half ? length_value * 1.5 : length_value;
}

inline uint8_t StringToNote(const std::string& str) {
	uint8_t pos = 0;
	uint8_t base = kNotesToInt.find(str[pos++])->second;
	if (str[pos] == 'b' || str[pos] == '#') {
		base += str[pos++] == '#' ? +1 : -1;
	}

	uint8_t octave = std::atoi(str.data() + pos);

	return base + octave * 12;
}

} // namespace

PhraseValue PhraseLiteralToValue(const ast::PhraseLiteral* phrase) {
	return {
		atic::MapIterable<decltype(phrase->notes), std::list<uint8_t> >(
			phrase->notes, [](const ast::IdentifierPtr& id) {
				return StringToNote(id->name);
			}),
		LengthToFloat(phrase->length),
	};
}

}  // namespace melo::evaluator
