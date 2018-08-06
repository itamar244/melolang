#include "melo/evaluator/module.h"
#include "melo/evaluator/values.h"
#include "evaluator/utils.h"

namespace melo::evaluator {

Module::Module(ast::BlockPtr&& program)
		: program_(std::move(program)) {
	for (auto& statement : program_->statements) {
		if (const auto& export_decl = statement->AsExport()) {
			std::pair entry = {
				export_decl->id->name,
				ExpressionToValue(export_decl->value),
			};
			exports_.insert(entry);
			top_scope_.insert(entry);
		} else {
			AddNodeToScope(statement);
		}
	}
}

void Module::AddNodeToScope(const ast::StatementPtr& statement) {
	switch (statement->type) {
		case ast::kFunctionDeclaration: {
			auto func = statement->AsFunctionDeclaration();
			top_scope_.insert({
				func->id->name,
				new FunctionValue(func->body),
			});
			break;
		}
	}
}

}  // namespace melo::evaluator
