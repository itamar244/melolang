#include "evaluator/evaluate_nodes.h"
#include <memory>
#include "evaluator/utils.h"

namespace melo::evaluator {

Scope::Value EvaluateExpr(Scope& scope, const ast::Expression* expr) {
	switch (expr->type) {
		case ast::kNumericLiteral:
			return Scope::NewNumberValue(
					std::atof(expr->AsNumericLiteral()->value.c_str()));
		case ast::kListLiteral:
			return Scope::NewListLiteralValue(expr->AsListLiteral());
		case ast::kIdentifier:
			return scope.Get(expr->AsIdentifier()->name);
		case ast::kFunctionCall:
			return EvaluateFunction(
				scope,
				scope.Get(expr->AsFunctionCall()->id->name)->ExpectFunctionValue());
		case ast::kPhraseLiteral:
			return Scope::NewPhraseValue(expr->AsPhraseLiteral());
		default:
			throw std::logic_error(
				"expression type isn't supported as value" +
				std::to_string(int(expr->type)));
	}
}

Scope::Value EvaluateFunction(Scope& scope, const FunctionValue* func) {
	Scope local_scope(&scope);

	for (const auto& statement : func->body->statements) {
		switch (statement->type) {
			case ast::kReturn:
				return EvaluateExpr(local_scope, statement->AsReturn()->expr);
			default:
				break;
		}
	}

	return Scope::NewNullValue();
}

} // namespace melo::evaluator
