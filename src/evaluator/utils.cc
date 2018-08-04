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

ast::ExpressionPtr& GetMain(ast::BlockPtr& program) {
	for (auto& statement : program->statements) {
		if (auto export_decl = statement->AsExport()) {
			if (export_decl->id->name == "main") {
				return export_decl->value;
			}
		}
	}

	throw std::logic_error("no 'main' function in ast");
}

Phrase PhraseNodeToPhrase(const ast::PhraseLiteralPtr& phrase) {
	return {
		atic::MapIterable<decltype(phrase->notes), std::list<uint8_t> >(
			phrase->notes, [](const ast::IdentifierPtr& id) {
				return StringToNote(id->name);
			}),
			LengthToFloat(phrase->length),
	};
}

Value* ExpressionToValue(const ast::ExpressionPtr& expr) {
	switch (expr->type) {
		case ast::kNumericLiteral:
			return new NumberValue(
					std::atof(expr->AsNumericLiteral()->value.c_str()));
		case ast::kSection:
			return new SectionValue(expr->AsSection());
		default:
			throw std::logic_error("expression type isn't supported as value");
	}
}

}  // namespace melo::evaluator
