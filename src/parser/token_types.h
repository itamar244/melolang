#pragma once

#include <map>
#include <string>

namespace melo::parser {

#define MELO_KEYWORD_TYPES(V)                                                  \
	V(func)                                                                      \
	V(return)                                                                    \
	V(export)                                                                    \
	V(var)

#define MELO_TOKEN_TYPES(V)                                                    \
	V(num)                                                                       \
	V(name)                                                                      \
	                                                                             \
	V(semi)                                                                      \
	V(comma)                                                                     \
	V(eq)                                                                        \
	V(bracketL)                                                                  \
	V(bracketR)                                                                  \
	V(braceL)                                                                    \
	V(braceR)                                                                    \
	V(parenL)                                                                    \
	V(parenR)                                                                    \
	                                                                             \
	V(eof)                                                                       \
	V(error)

enum class TokenType {
#define V(KEYWORD) _##KEYWORD,
	MELO_KEYWORD_TYPES(V)
#undef V

#define V(TOKEN) TOKEN,
	MELO_TOKEN_TYPES(V)
#undef V
};

using tt = TokenType;

const std::map<std::string, TokenType> keywords({
#define V(KEYWORD) {#KEYWORD, tt::_##KEYWORD},
	MELO_KEYWORD_TYPES(V)
#undef V
});

std::string TokenTypeToString(TokenType);

} // namespace melo::parser
