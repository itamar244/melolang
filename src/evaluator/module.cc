#include "melo/evaluator/module.h"
#include <atic/ptr.h>
#include "melo/evaluator/evaluate_nodes.h"
#include "melo/evaluator/values.h"
#include "evaluator/utils.h"

namespace melo::evaluator {

Module::Module(ast::BlockPtr&& program)
		: program_(std::move(program))
		, exports_(Scope())
		, top_scope_(Scope()) {
	for (auto& statement : program_->statements) {
		if (const auto& export_decl = statement->AsExport()) {
			std::pair entry = {
				export_decl->id->name,
				EvaluateExpr(top_scope_, export_decl->value),
			};
			exports_.Set(entry.first, entry.second);
			top_scope_.Set(entry.first, entry.second);
		} else {
			AddNodeToScope(statement);
		}
	}
}

Module::~Module() {
	for (auto& item : exports_.scope()) atic::DeletePtr(item.second);
	// top exports items could be deleted from exports_
	for (auto& item : top_scope_.scope()) atic::MaybeDeletePtr(item.second);
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
