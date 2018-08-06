#include "melo/evaluator/values.h"
#include <exception>

namespace melo::evaluator {

Value* ExpressionToValue(const ast::ExpressionPtr& expr) {
	switch (expr->type) {
		case ast::kNumericLiteral:
			return new NumberValue(
					std::atof(expr->AsNumericLiteral()->value.c_str()));
		case ast::kListLiteral:
			return new ListLiteralValue(expr->AsListLiteral());
		default:
			throw std::logic_error("expression type isn't supported as value");
	}
}

}  // namespace melo::evaluator
