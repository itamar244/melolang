#pragma once

#include <string>
#include <exception>
#include <memory>
#include "parser/state.h"
#include "parser/token_types.h"

namespace melo::parser {

class Tokenizer {
public:
	Tokenizer(State::Ptr state)
		:	state_{state} {}

	void Next();

	inline bool Match(TokenType type) const {
		return type == state_->type;
	}

	inline void Expect(TokenType type) const noexcept(false) {
		if (!Match(type)) {
			throw std::logic_error(
				"wrong type: " + TokenTypeToString(state_->type) +
				", expected: " + TokenTypeToString(type)
			);
		}
	}

	inline void ExpectAndNext(TokenType type) {
		Expect(type);
		Next();
	}

	// inline void ExpectNext(TokenType type) {
	// 	Next();
	// 	Expect(type);
	// }

	inline bool Eat(TokenType type) {
		if (Match(type)) {
			Next();
			return true;
		}
		return false;
	}

protected:
	State::Ptr state_;

private:
	void SkipLine();
	void GetTokenFromChar(char);
	void ReadDot();
	void ReadIdentifier();
	void ReadNumber();
	void FinishToken(TokenType, const std::string& value = "");
	void FinishTokenWithValue(TokenType);
	void SkipSpace();
};

} // namespace melo::parser
