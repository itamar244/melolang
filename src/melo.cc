#include <istream>
#include "melo/melo.h"
#include "parser/parser.h"
#include "parser/state.h"

namespace melo {

ast::BlockPtr Parse(std::istream& stream) {
	return parser::Parser(parser::State::Create(stream)).Parse();
}

evaluator::Module CreateModule(std::istream& stream) {
	return {Parse(stream)};
}

} // namespace melo
