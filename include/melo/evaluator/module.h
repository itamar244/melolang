#pragma once

#include <exception>
#include <map>
#include <memory>
#include <string>
#include "melo/ast.h"
#include "melo/phrase.h"
#include "melo/evaluator/section_walker.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

class Module {
public:
	Module(ast::BlockPtr&& program);

	inline evaluator::SectionWalker& main_walker() {
		return main_walker_;
	}
	inline const evaluator::SectionWalker& main_walker() const {
		return main_walker_;
	}
	// FIXME:
	inline const Value* Get(const std::string name) {
		auto pair = exports_.find(name);
		if (pair == exports_.end()) {
			throw std::logic_error("no such '" + name + "' export");
		}
		return pair->second;
	}

private:
	std::map<std::string, Value*> exports_;
	ast::BlockPtr program_;
	evaluator::SectionWalker main_walker_;
};

}  // namespace melo::evaluator
