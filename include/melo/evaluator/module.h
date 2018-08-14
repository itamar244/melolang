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
	~Module();

	inline const Value* GetExport(const std::string& name) {
		auto& scope = exports_.scope();
		auto pair = scope.find(name);
		if (pair == scope.end()) {
			throw std::logic_error("no such '" + name + "' export");
		}
		return pair->second;
	}

	inline SectionWalker GetMain() {
		return {top_scope_, GetExport("main")->ExpectListLiteralValue()};
	}

private:
	LinkedScope top_scope_;
	LinkedScope exports_;
	ast::BlockPtr program_;

	void AddNodeToScope(const ast::StatementPtr& statement);
};

}  // namespace melo::evaluator
