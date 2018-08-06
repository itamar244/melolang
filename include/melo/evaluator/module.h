#pragma once

#include <exception>
#include <map>
#include <memory>
#include <string>
#include "melo/ast.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

class Module {
public:
	using Scope = std::map<std::string, Value*>;

	Module(ast::BlockPtr&& program);

	inline const Value* GetExport(const std::string& name) {
		auto pair = exports_.find(name);
		if (pair == exports_.end()) {
			throw std::logic_error("no such '" + name + "' export");
		}
		return pair->second;
	}

private:
	Scope top_scope_;
	Scope exports_;
	ast::BlockPtr program_;

	void AddNodeToScope(const ast::StatementPtr& statement);
};

}  // namespace melo::evaluator
