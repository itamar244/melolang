#pragma once

#include <exception>
#include <memory>
#include "melo/ast.h"
#include "melo/evaluator/scope.h"
#include "melo/evaluator/section_walker.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

class Module {
public:
	Module(ast::BlockPtr&& program);

	inline const Value* GetExport(const std::string& name) {
		auto pair = exports_.find(name);
		if (pair == exports_.end()) {
			throw std::logic_error("no such '" + name + "' export");
		}
		return pair->second;
	}

	inline SectionWalker GetMain() {
		return {top_scope_, GetExport("main")->ExpectListLiteralValue()};
	}

private:
	Scope top_scope_;
	Scope::Data exports_;
	ast::BlockPtr program_;

	void AddNodeToScope(const ast::StatementPtr& statement);
};

}  // namespace melo::evaluator
