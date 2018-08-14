#include "melo/evaluator/evaluate_nodes.h"
#include <memory>
#include "evaluator/utils.h"

namespace melo::evaluator {

Value* EvaluateExpr(Scope& scope, const ast::ExpressionPtr& expr) {
  switch (expr->type) {
    case ast::kNumericLiteral:
      return new NumberValue(
          std::atof(expr->AsNumericLiteral()->value.c_str()));
    case ast::kListLiteral:
      return new ListLiteralValue(expr->AsListLiteral());
    case ast::kFunctionCall:
      return EvaluateFunction(
          scope,
          scope.Get(expr->AsFunctionCall()->id->name)->ExpectFunctionValue());
    case ast::kPhraseLiteral:
      return new PhraseValue(
          std::move(PhraseLiteralToValue(expr->AsPhraseLiteral())));
    default:
      throw std::logic_error(
          "expression type isn't supported as value" +
          std::to_string(int(expr->type)));
  }
}

Value* EvaluateFunction(Scope& scope, const FunctionValue* func) {
  Scope local_scope(&scope);

  for (const auto& statement  : func->body->statements) {
    switch (statement->type) {
      case ast::kReturn:
        return EvaluateExpr(local_scope, statement->AsReturn()->expr);
      default:
        break;
    }
  }

  return new NullValue();
}

} // namespace melo::evaluator
