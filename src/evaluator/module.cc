#include "melo/evaluator/module.h"
#include <atic/ptr.h>
#include "evaluator/evaluate_nodes.h"
#include "melo/evaluator/values.h"
#include "evaluator/utils.h"

namespace melo::evaluator {

Module::Module(ast::Block* program, Zone* zone)
		: program_(program), zone_(zone) {
	for (const auto& statement : program_->statements) {
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

Module::~Module() {
	delete zone_;
}

void Module::AddNodeToScope(const ast::Statement* statement) {
	switch (statement->type) {
		case ast::kFunctionDeclaration: {
			auto func = statement->AsFunctionDeclaration();
			top_scope_.Set(func->id->name, new FunctionValue(func->body));
			break;
		}
		case ast::kVariableDeclaration: {
			auto var = statement->AsVariableDeclaration();
			top_scope_.Set(var->id->name, EvaluateExpr(top_scope_, var->val));
			break;
		}
		default:
			break;
	}
}

}  // namespace melo::evaluator
