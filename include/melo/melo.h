#pragma once

#include <istream>
#include "ast.h"
#include "evaluator/module.h"
#include "parser/parser.h"
#include "parser/state.h"

namespace melo {

inline ast::BlockPtr Parse(std::istream& stream) {
	return parser::Parser(parser::State::Create(stream)).Parse();
}

inline evaluator::Module CreateModule(std::istream& stream) {
	return {Parse(stream)};
}

} // namespace melo
