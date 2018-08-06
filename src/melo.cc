#include <istream>
#include "melo/melo.h"
#include "parser/parser.h"
#include "parser/state.h"

namespace melo {

ast::BlockPtr Parse(std::istream& stream) {
	return parser::Parser(parser::State::Create(stream)).Parse();
}

} // namespace melo
