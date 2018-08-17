#pragma once

#include <istream>
#include <memory>
#include "melo/ast.h"
#include "melo/evaluator/module.h"
#include "melo/evaluator/section_walker.h"
#include "melo/evaluator/values.h"

namespace melo {

// exports for easy use
using evaluator::Module;
using evaluator::SectionWalker;
using evaluator::PhraseValue;

ast::BlockPtr Parse(std::istream& stream);

inline Module CreateModule(std::istream& stream) {
	return {Parse(stream)};
}
inline Module CreateModule(ast::BlockPtr&& program) {
	return {std::move(program)};
}

} // namespace melo
