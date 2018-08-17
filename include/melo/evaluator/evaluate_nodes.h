#pragma once

#include "melo/ast.h"
#include "melo/evaluator/scope.h"
#include "melo/evaluator/values.h"

namespace melo::evaluator {

Value* EvaluateExpr(Scope& scope, const ast::Expression* expr);
Value* EvaluateFunction(Scope& scope, const FunctionValue* func);

} // namespace melo::evaluator
