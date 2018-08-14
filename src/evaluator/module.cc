#include "melo/evaluator/module.h"
#include <atic/ptr.h>
#include "melo/evaluator/evaluate_nodes.h"
#include "melo/evaluator/values.h"
#include "evaluator/utils.h"

namespace melo::evaluator {

Module::Module(ast::BlockPtr&& program)
		: program_(std::move(program)) {
	for (auto& statement : program_->statements) {
		if (const auto& export_decl = statement->AsExport()) {
			const auto& name = export_decl->id->name;
			auto value = EvaluateExpr(top_scope_, export_decl->value);
			exports_.insert({ name, value });
			top_scope_.Set(name, value);
		} else {
			AddNodeToScope(statement);
		}
	}
}

void Module::AddNodeToScope(const ast::StatementPtr& statement) {
	switch (statement->type) {
		case ast::kFunctionDeclaration: {
			auto func = statement->AsFunctionDeclaration();
			top_scope_.Set(func->id->name, new FunctionValue(func->body.get()));
			break;
		}
	}
}

}  // namespace melo::evaluator
