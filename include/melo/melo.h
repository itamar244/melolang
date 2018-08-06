#pragma once

#include <istream>
#include <memory>
#include "melo/ast.h"
#include "melo/evaluator/module.h"

namespace melo {

ast::BlockPtr Parse(std::istream& stream);

inline evaluator::Module CreateModule(std::istream& stream) {
	return {Parse(stream)};
}
inline evaluator::Module CreateModule(ast::BlockPtr&& program) {
	return {std::move(program)};
}

} // namespace melo
