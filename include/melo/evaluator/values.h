#pragma once

#include <exception>
#include <list>
#include <map>
#include <memory>
#include <string>
#include "melo/ast.h"
#include "melo/zone.h"

namespace melo::evaluator {

#define MELO_EVALUATOR_VALUE_TYPES(V)                                          \
	V(NullValue)                                                                 \
	V(FunctionValue)                                                             \
	V(ListLiteralValue)                                                          \
	V(PhraseValue)                                                               \
	V(NumberValue)

enum ValueType : uint8_t {
#define DECLARE_TYPE_ENUM(TYPE) k##TYPE,
	MELO_EVALUATOR_VALUE_TYPES(DECLARE_TYPE_ENUM)
#undef DECLARE_TYPE_ENUM
};

#define FR_DECL(NAME)	                                                         \
	struct NAME;                                                                 \

	MELO_EVALUATOR_VALUE_TYPES(FR_DECL)
#undef FR_DECL

struct Value : public ZoneObject {
	using ZoneObject::operator new;
	const ValueType type;

#define V(NAME)                                                                \
	inline const NAME* As##NAME() const {                                        \
		return type == k##NAME ? reinterpret_cast<const NAME*>(this) : nullptr;    \
	}                                                                            \
	inline const NAME* Expect##NAME() const {                                    \
		if (type != k##NAME) throw std::logic_error("wrong type");                 \
		return reinterpret_cast<const NAME*>(this);                                \
	}                                                                            \
	inline bool Is##NAME() const { return As##NAME() != nullptr; }

	MELO_EVALUATOR_VALUE_TYPES(V)
#undef V

protected:
	Value(ValueType type) : type(type) {}
};

struct NullValue : public Value {
	NullValue() : Value(kNullValue) {}
};

struct FunctionValue : public Value {
	// FIXME: add params when added
	const ast::Block* body;

	FunctionValue(const ast::Block* body)
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

	inline std::size_t size() const { return list->elements.size(); }
	inline auto begin() const { return list->elements.begin(); }
	inline auto end() const { return list->elements.end(); }
};

struct PhraseValue : public Value {
	const ast::PhraseLiteral* phrase;

	PhraseValue(const ast::PhraseLiteral* phrase)
			: Value(kPhraseValue), phrase(phrase) {}
};

}  // namespace melo::evaluator
