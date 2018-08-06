#pragma once

#include <memory>
#include "melo/ast.h"
#include "melo/phrase.h"

namespace melo::evaluator {

#define MELO_EVALUATOR_VALUE_TYPES(V)                                          \
	V(FunctionValue)                                                             \
	V(ListLiteralValue)                                                          \
	V(NumberValue)

enum ValueType : uint8_t {
#define DECLARE_TYPE_ENUM(TYPE) k##TYPE,
	MELO_EVALUATOR_VALUE_TYPES(DECLARE_TYPE_ENUM)
#undef DECLARE_TYPE_ENUM
};

#define FR_DECL(NAME)	struct NAME;
	MELO_EVALUATOR_VALUE_TYPES(FR_DECL)
#undef FR_DECL

struct Value {
	const ValueType type;

#define V(NAME)                                                                \
	inline const NAME* As##NAME() const {                                        \
		return type == k##NAME ? reinterpret_cast<const NAME*>(this) : nullptr;    \
	}                                                                            \
	inline bool Is##NAME() const { return As##NAME() != nullptr; }

	MELO_EVALUATOR_VALUE_TYPES(V)
#undef V

protected:
	Value(ValueType type) : type(type) {}
};


Value* ExpressionToValue(const ast::ExpressionPtr& expr);


struct FunctionValue : public Value {
	// FIXME: add params when added
	const ast::BlockPtr& body;

	FunctionValue(const ast::BlockPtr& body)
			: Value(kFunctionValue), body(body) {}
};

struct NumberValue : public Value {
	const float value;

	NumberValue(float value) : Value(kNumberValue), value(value) {}
};

struct ListLiteralValue : public Value {
	const ast::ListLiteral* list;

	ListLiteralValue(const ast::ListLiteral* list)
			: Value(kListLiteralValue), list(list) {}

	inline const Value* operator[](std::size_t i) const {
		return ExpressionToValue(list->elements.at(i));
	}
};

}  // namespace melo::evaluator
