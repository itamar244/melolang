#pragma once

#include <cstdint>
#include <cstring>
#include <istream>
#include <memory>
#include <string>
#include "parser/token_types.h"

namespace melo::parser {

struct State {
	using Ptr = std::shared_ptr<State>;

	static Ptr Create(std::istream& input) {
		return std::make_shared<State>(input);
	}

	std::istream& input;
	TokenType type = tt::eof;
	uint32_t pos = 0;
	uint32_t line = 0;
	uint32_t start = 0;
	uint32_t lineStart = 0;
	std::string value = "";

	State(std::istream& input) : input(input) {}

	inline bool ended() {
		input.seekg(pos);
		return input.peek() == -1;
	}

	inline char CurChar() {
		// ended already seeks position
		return ended() ? '\x00' : input.peek();
	}

	inline std::string CurValue() {
		uint32_t size = pos - start;
		char buffer[size];
		input.seekg(start);
		input.read(buffer, size);
		return std::string(buffer, size);
	}
};

} // namespace melo::parser
