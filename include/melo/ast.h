#pragma once

#include <cctype>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "melo/list.h"
#include "melo/zone.h"
#include "melo/string.h"

namespace melo::ast {

#define MELO_WRAPPERS_NODE_TYPES(V)                                            \
	V(Block)

#define MELO_STATEMENT_NODE_TYPES(V)                                           \
	V(FunctionDeclaration)                                                       \
	V(Export)                                                                    \
	V(Return)

#define MELO_EXPRESSION_NODE_TYPES(V)                                          \
	V(ListLiteral)                                                               \
	V(PhraseLiteral)                                                             \
	V(Identifier)                                                                \
	V(FunctionCall)                                                              \
	V(NumericLiteral)                                                            \
	V(Spread)

#define MELO_AST_NODE_TYPES(V)                                                 \
	MELO_WRAPPERS_NODE_TYPES(V)                                                  \
	MELO_STATEMENT_NODE_TYPES(V)                                                 \
	MELO_EXPRESSION_NODE_TYPES(V)

enum NodeType : uint8_t {
#define DECLARE_TYPE_ENUM(TYPE) k##TYPE,
	MELO_AST_NODE_TYPES(DECLARE_TYPE_ENUM)
#undef DECLARE_TYPE_ENUM
};

#define FR_DECL(NAME) struct NAME;

	MELO_AST_NODE_TYPES(FR_DECL)
#undef FR_DECL

struct AstNode : public ZoneObject {
	using ZoneObject::operator new;
	const NodeType type;

#define V(NAME)                                                                \
	inline const NAME* As##NAME() const  {                                       \
		return type == k##NAME ? reinterpret_cast<const NAME*>(this) : nullptr;    \
	}                                                                            \
	inline bool Is##NAME() const { return As##NAME() != nullptr; }

	MELO_AST_NODE_TYPES(V)
#undef V

protected:
	AstNode(NodeType type) : type(type) {}

private:
	// for accidently preventing call `operator new` without Zone*
	void* operator new(std::size_t size);
};

struct Statement : public AstNode {
	Statement(NodeType type) : AstNode(type) {}
};

struct Expression : public AstNode {
	Expression(NodeType type) : AstNode(type) {}
};

struct Identifier : public Expression {
	const String name;

	Identifier(String&& name) : Expression(kIdentifier), name(std::move(name)) {}
};

struct ListLiteral : public Expression {
	const List<Expression*> elements;

	ListLiteral(List<Expression*> elements)
			: Expression(kListLiteral), elements(elements) {}
};

struct NumericLiteral : public Expression {
	const String value;

	NumericLiteral(String&& value)
			: Expression(kNumericLiteral), value(std::move(value)) {}
};

struct PhraseLiteral : public Expression {
	const NumericLiteral* length;
	const List<Identifier*> notes;

	PhraseLiteral(
			const NumericLiteral* length, const List<Identifier*>& notes)
			: Expression(kPhraseLiteral)
			, length(length)
			, notes(notes) {}
};

struct Spread : public Expression {
	const Expression* value;

	Spread(Expression* value) : Expression(kSpread), value(value) {}
};

struct FunctionCall : public Expression {
	const Identifier* id;
	const List<Expression*> args;

	FunctionCall(const Identifier* id, const List<Expression*>& args)
			: Expression(kFunctionCall)
			, id(id)
			, args(args) {}
};

struct Block : public Statement {
	const List<const Statement*> statements;

	Block(const List<const Statement*>& statements)
			: Statement(kBlock), statements(statements) {}
};

struct Export : public Statement {
	const Identifier* id;
	const Expression* value;

	Export(const Identifier* id, const Expression* value)
			: Statement(kExport)
			, id(id)
			, value(value) {}
};

struct FunctionDeclaration : public Statement {
	const Identifier* id;
	const List<Identifier*> params;
	const Block* body;

	FunctionDeclaration(
			const Identifier* id, const List<Identifier*>& params, const Block* body)
			: Statement(kFunctionDeclaration)
			, id(id)
			, params(params)
			, body(body) {}
};

struct Return : public Statement {
	const Expression* expr;

	Return(Expression* expr) : Statement(kReturn), expr(expr) {}
};


class NodeFactory {
public:
	NodeFactory(Zone* zone) : zone_(zone) {}

	template<typename T>
	inline List<T> CreateList() {
		return {zone_};
	}

#define NODE_FACTORIES(NAME)                                                   \
 	template<typename... Args>                                                   \
	NAME* New##NAME(const Args&... args) {                                       \
		return new (zone_) NAME(args...);                                          \
	}

	MELO_AST_NODE_TYPES(NODE_FACTORIES)
#undef NODE_FACTORIES

private:
	Zone* zone_;
};

std::string NodeTypeToString(NodeType);

} // namespace melo::ast
