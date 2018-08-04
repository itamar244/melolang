#include "./token_types.h"

namespace melo::parser {

const std::string TokenTypeToString(TokenType type) {
	switch (type) {
#define V(KEYWORD) case tt::_##KEYWORD: return #KEYWORD;
		MELO_KEYWORD_TYPES(V)
#undef V

#define V(TOKEN) case tt::TOKEN: return #TOKEN;
		MELO_TOKEN_TYPES(V)
#undef V
	}
}

} // namespace melo::parser
