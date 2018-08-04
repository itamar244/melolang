#pragma once

#include <string>
#include <exception>
#include <memory>
#include "./state.h"
#include "./token_types.h"

namespace melo::parser {

class Tokenizer {
public:
	Tokenizer(State::Ptr state)
		:	state_{state} {}

	void Next();

	inline bool Match(TokenType type) {
		return type == state_->type;
	}

	inline void Expect(TokenType type) noexcept(false) {
		if (!Match(type)) {
			throw std::logic_error(
				"wrong type: " + TokenTypeToString(state_->type) +
				", expected: " + TokenTypeToString(type)
			);
		}
	}

	inline bool Eat(TokenType type) {
		if (Match(type)) {
			Next();
			return true;
		}
		return false;
	}

private:
	State::Ptr state_;

	void SkipLine();
	void ReadIdentifier();
	void ReadNumber();
	void GetTokenFromChar(char);
	void FinishToken(TokenType, const std::string& value = "");
	void FinishTokenWithValue(TokenType);
	void SkipSpace();
};

} // namespace melo::parser
