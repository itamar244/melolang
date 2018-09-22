#pragma once

#include <istream>
#include <memory>
#include "melo/ast.h"
#include "melo/evaluator/module.h"
#include "melo/evaluator/section_walker.h"
#include "melo/zone.h"

namespace melo {

// exports for easy use
using evaluator::Module;
using evaluator::SectionWalker;

ast::Block* Parse(std::istream& stream, Zone* zone);

inline Module CreateModule(std::istream& stream) {
	auto zone = new Zone();
	return {Parse(stream, zone), zone};
}

inline Module CreateModule(ast::Block* program) {
	return {program, nullptr};
}

} // namespace melo
