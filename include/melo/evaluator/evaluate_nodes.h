#pragma once

#include "melo/ast.h"
#include "melo/evaluator/scope.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

Value* EvaluateExpr(LinkedScope& scope, const ast::ExpressionPtr& expr);
Value* EvaluateFunction(LinkedScope& scope, const FunctionValue* func);

} // namespace melo::evaluator
