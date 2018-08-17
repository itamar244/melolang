#include <istream>
#include "melo/melo.h"
#include "melo/zone.h"
#include "parser/parser.h"
#include "parser/state.h"

namespace melo {

ast::Block* Parse(std::istream& stream, Zone* zone) {
	return parser::Parser(parser::State::Create(stream), zone).Parse();
}

} // namespace melo
